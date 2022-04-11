import os
from symtable import Symbol
from urllib import response

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, gen_portfolio, login_required, lookup, usd, gen_portfolio

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # get all the users transactions
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = ?", session["user_id"])

    # convert list of transactions into portfolio
    portfolio = gen_portfolio(transactions)

    # calculate total portfolio value
    total =sum([p['total'] for p in portfolio])
    sums = {'total': usd(total)}

    # Get the users cash balance
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session['user_id'])[0]['cash']
    sums['cash'] = usd(cash)
    sums['totaltotal'] = usd(cash + total)
    # convert currencies to be displayed as USD strings
    for p in portfolio:
        p['total'] = usd(p['total'])
        p['price'] = usd(p['price'])

    return render_template("index.html", portfolio=portfolio, sums=sums)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        # ERROR CHECKING INPUTS
        symbol = request.form.get("symbol")

        try:
            shares = int(request.form.get("shares"))
        except ValueError:
            return apology("Thats not an interger number of shares bro!")

        quote = lookup(symbol)
        if quote == None:
            return apology("Invalid Stock Symbol!")

        if not shares or shares == 0:
            return apology("Enter a sensible share amount")

        if shares < 0:
            return apology("U can't buy negative shares idiot!")

        # Check if user can afford transaction
        balance = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]['cash']

        if balance < shares * quote['price']:
            return apology("Find some more money first dude")

        # RECORD TRANSACTION IN SQL DATABASE
        db.execute("INSERT INTO transactions (symbol, shares, price, user_id) VALUES(?, ?, ?, ?)", symbol, shares, quote["price"], session["user_id"])

        # Reduce users balance by share price * shares
        db.execute("UPDATE users SET cash = ? WHERE id = ?", balance - shares * quote['price'], session['user_id'])
        return redirect("/")



    if request.method == "GET":
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # get all transactions from user
    transactions = db.execute('SELECT * FROM transactions WHERE user_id = ?', session['user_id'])
    return render_template('history.html', transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

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
        symbol = request.form.get("symbol")

        quote = lookup(symbol)
        if quote == None:
            return apology("Invalid Stock Symbol!")

        price = usd(quote["price"])

        return render_template("quote.html", quote=quote, price=price)

    if request.method == "GET":
        return render_template("quote.html", quote=None, price=None)


@app.route("/register", methods=["GET", "POST"])
def register():
    # if via post then user wants to be registered
    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")
        passwordConfirm = request.form.get("passwordConfirm")

        #ensure username was submitted and does not exist
        if not username:
            return apology("Enter a Username!")

        print(db.execute("SELECT username FROM users"))
        if db.execute("SELECT * FROM users WHERE username = ?", username):
            return apology("Username is already taken!")

        # ensure password was submitted and matches its confirmation
        if not password:
            return apology("enter a password dumb dumb")

        if password != passwordConfirm:
            return apology("passwords do not match!!!")

        # Add username and hashed password to database

        db.execute("INSERT INTO users (username, hash) VALUES(?, ?)", username, generate_password_hash(password))
        return render_template("register.html")


    # else if reached via GET (by clicking link, show register page)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # Get all symbols onwed by user
    portfolio = gen_portfolio(db.execute("SELECT * FROM transactions WHERE user_id = ?", session['user_id']))
    symbols = [p['symbol'] for p in portfolio]
    sharesOwned = [{'symbol': p['symbol'], 'shares': p['shares']} for p in portfolio]

    if request.method == 'POST':
        # get symbol and shares from form (and input error check)
        symbol = request.form.get('symbol')
        try:
            sharesSell = int(request.form.get('shares'))
        except ValueError:
            return apology("USE A NUMBER MAN!")

        if symbol not in symbols:
            return apology("PICK A VALID SYMBOL! SMH")

        if sharesSell > [s for s in sharesOwned if s['symbol'] == symbol][0]['shares']:
            return apology("CAN'T SELL WHAT YOU DON'T HAVE")

        if sharesSell == 0:
            return apology("CAN'T SELL NOTHING")

        # get quote
        quote = lookup(symbol)

        # RECORD TRANSACTION IN SQL DATABASE (negative shares for selling)
        db.execute("INSERT INTO transactions (symbol, shares, price, user_id) VALUES(?, ?, ?, ?)", symbol, -1*sharesSell, quote["price"], session["user_id"])

        # Increase users balance by share price * shares
        cash = db.execute('SELECT cash FROM users WHERE id = ?', session['user_id'])[0]['cash']
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash + sharesSell * quote['price'], session['user_id'])

        return redirect("/")

    if request.method == 'GET':
        # Populate the drop down menu with all symbol options
        return render_template("sell.html", symbols=symbols)
