# Secure Flask Login Application

A Flask-based web application that implements secure user authentication with salt and pepper password hashing techniques. This application uses SQLite for database storage and features a responsive Bootstrap UI.

## Security Features

* **Salt and Pepper Password Protection** : Each password is secured with both a unique salt (per user) and a global pepper.
* **PBKDF2 with SHA-256** : Industry-standard key derivation function for password hashing.
* **High Iteration Count** : Uses 100,000 iterations for added security against brute force attacks.
* **Secure Session Management** : Flask sessions are cryptographically signed.
* **Password Strength Requirements** : Enforces strong password policies.
* **CSRF Protection** : Built-in Cross-Site Request Forgery protection.

## Prerequisites

* Python 3.7 or higher
* pip (Python package installer)

## Installation

1. Clone the repository (or download the files):

```bash
git clone https://github.com/yourusername/secure-flask-app.git
cd secure-flask-app
```

2. Create and activate a virtual environment:

```bash
python -m venv venv

# On Windows
venv\Scripts\activate

# On macOS/Linux
source venv/bin/activate
```

3. Install the required packages:

```bash
pip install flask flask-sqlalchemy
```

4. Set up the environment variable for the pepper value (recommended for production):

```bash
# On Windows
set PASSWORD_PEPPER=your_secure_random_string

# On macOS/Linux
export PASSWORD_PEPPER=your_secure_random_string
```

## Running the Application

1. Start the Flask development server:

```bash
python app.py
```

2. Open your web browser and navigate to:

```
http://127.0.0.1:5000/
```

## Project Structure

```
secure-flask-app/
├── app.py                 # Main application file
├── users.db               # SQLite database (created on first run)
├── templates/             # HTML templates
│   ├── base.html          # Base template with layout
│   ├── index.html         # Home page
│   ├── login.html         # Login page
│   ├── register.html      # Registration page
│   ├── dashboard.html     # User dashboard
│   └── 404.html           # Error page
└── README.md              # This file
```

## Deployment

For production deployment:

1. Use a production WSGI server like Gunicorn or uWSGI
2. Set `app.config['SECRET_KEY']` to a secure random value
3. Set the `PASSWORD_PEPPER` environment variable
4. Disable debug mode by setting `debug=False`
5. Consider using a production-grade database like PostgreSQL

## Salt and Pepper Explanation

This application uses both salt and pepper techniques for password security:

* **Salt** : A unique random value generated for each user and stored in the database alongside the password hash. Prevents rainbow table attacks.
* **Pepper** : A secret value that is not stored in the database (ideally kept in environment variables or a separate secure storage). Adds an extra layer of protection if the database is compromised.

The combination of both techniques provides significantly stronger security than using either one alone.

## License

MIT License
