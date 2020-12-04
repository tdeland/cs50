import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    # show portfolio of stocks

    # get list of user's stocks, excluding stocks with zero shares
    stocks = db.execute("SELECT * FROM stocks WHERE user_id = :userid AND shares > 0",
        userid=session["user_id"])

    #initialize grand total
    total = getCash()

    for row in stocks:

        # get lastest information about stock and add to row
        info = lookup(row["symbol"])
        row["price"] = info["price"]
        row["name"] = info["name"]

        # calculate stock value subtotal
        row["total"] = row["shares"] * row["price"]

        # update grand total (cash + stocks)
        total += row["total"]
        row["total"] = usd(row["total"])
        row["price"] = usd(row["price"])

    return render_template("portfolio.html", cash=usd(getCash()), stocks=stocks, total=usd(total))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    # Buy shares of stock
    if request.method == "POST":

        # get transaction info
        bought = int(request.form.get("shares"))

        # calculate remaining cash
        cashNow = getCash() - session["quoted"]["price"] * bought

        # update database with transaction info
        db.execute("UPDATE users SET cash = :cash WHERE id = :userid",
            cash=cashNow,
            userid=session["user_id"])

        # check if user already owns purchased stock
        stock = db.execute("SELECT * FROM stocks WHERE symbol = :symbol AND user_id = :userid",
            symbol=session["quoted"]["symbol"],
            userid=session["user_id"])
        if not stock:
            # first-time purchase for user, add new entry to stocks table
            stockid=db.execute("INSERT INTO stocks (user_id, symbol, shares) VALUES (:userid, :symbol, :shares)",
                userid=session["user_id"],
                symbol=session["quoted"]["symbol"],
                shares=bought)
        else:
            # user already owns some stock, update number of shares
            stockid = stock[0]["id"]
            db.execute("UPDATE stocks SET shares = :shares WHERE id = :stock_id",
                shares=stock[0]["shares"] + bought,
                stock_id=stockid)

        # update transaction history table
        db.execute("INSERT INTO history (stock_id, price, shares) VALUES (:stock_id, :price, :shares)",
            stock_id=stockid,
            price=session["quoted"]["price"],
            shares=bought)
        return redirect("/")
    else:
        return render_template("quote.html")

@app.route("/history")
@login_required
def history():
    # Show history of transactions
    history = db.execute("SELECT stocks.symbol, history.shares, history.price, history.timestamp FROM history JOIN stocks ON (stock_id = stocks.id) WHERE stocks.user_id = :userid ORDER BY timestamp DESC", userid=session["user_id"])
    print(history)
    return render_template("history.html", history=history)

@app.route("/login", methods=["GET", "POST"])
def login():
    # log in user

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        # look up stock info and current price
        info = lookup(request.form.get("symbol"))

        # check valid symbol
        if not info:
            return render_template("quote.html")

        # save to session for future use
        session["quoted"] = info

        # display updated quote page
        return render_template("quoted.html", name=info["name"], symbol=info["symbol"], price=usd(info["price"]), max_shares=int(getCash()/info["price"]))

    else:
        # show blank form, leaving result table and buy option hidden
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure passwords match
        if request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords must match", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username doesn't already exist
        if len(rows) > 0:
            return apology("username already registered", 403)

        # Register user to database, set session user id
        session["user_id"] = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                   username=request.form.get("username"),
                   hash=generate_password_hash(request.form.get("password"),'pbkdf2:sha256', 8))

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    # Sell shares of stock
    if request.method == "POST":

        # get latest price
        info = lookup(request.form.get("symbolValue"))

        # get number of shares sold
        sold = int(request.form.get("shares"))

        # calculate total value
        total = info["price"] * sold

        # update stock table
        stock = db.execute("SELECT * FROM stocks WHERE symbol = :symbol AND user_id = :userid",
            symbol=info["symbol"],
            userid=session["user_id"])

        db.execute("UPDATE stocks SET shares = :shares WHERE id = :stock_id",
            shares=stock[0]["shares"] - sold,
            stock_id=stock[0]["id"])

        # update history
        db.execute("INSERT INTO history (stock_id, price, shares) VALUES (:stock_id, :price, :shares)",
            stock_id=stock[0]["id"],
            price=info["price"],
            shares=-sold)

        # update cash
        db.execute("UPDATE users SET cash = :cash WHERE id = :userid",
            cash=getCash() + total,
            userid=session["user_id"])

        return redirect("/")
    else:
        # get list of stocks user owns
        stocks = db.execute("SELECT symbol, shares from stocks WHERE user_id = :userid AND shares > 0",
            userid=session["user_id"])

        return render_template("sell.html", stocks=stocks)

def errorhandler(e):
    # Handle error
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)

def getCash():
    # get amount of free cash for current user
    cash = db.execute("SELECT cash FROM users WHERE id = :userid", userid=session["user_id"])
    return float(cash[0]["cash"])

# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
