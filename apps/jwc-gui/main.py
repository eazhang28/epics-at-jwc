from flask import Flask, request, render_template, jsonify
import os
import sqlite3
import shlex
import configparser
from werkzeug.security import generate_password_hash, check_password_hash
from waitress import serve

BASE_DIR   = os.path.dirname(os.path.abspath(__file__))
CONFIG_PATH = os.path.join(BASE_DIR, '..', '..', 'jwc_seq_config.ini')
template_dir = os.path.join(BASE_DIR, 'templates')
app = Flask(__name__, template_folder=template_dir)

def init_db():
    db_path = os.path.join(BASE_DIR, 'users.db')
    conn = sqlite3.connect(db_path)
    c = conn.cursor()
    c.execute('''CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT)''')
    conn.commit()
    conn.close()

init_db()

@app.route("/send_cmd", methods=['POST'])
def send_cmd():
    input_text = request.json['input']
    cmd = f"echo {shlex.quote(input_text)} > /tmp/input_pipe &"
    os.system(cmd)
    return {'status': 'sent'}

@app.route("/")
def home():
    return(render_template('index.html'))

@app.route("/write")
def write():
    return(render_template('write.html'))

@app.route("/about")
def about():
    return (render_template('about.html'))

@app.route("/login")
def login():
    return (render_template('login.html'))

@app.route("/confirm")
def confirm():
    return (render_template('confirm.html'))

@app.route("/dashboard")
def dashboard():
    return (render_template('dashboard.html'))

@app.route("/config")
def config_page():
    return render_template('config.html')

@app.route("/api/config", methods=['GET'])
def config_get():
    cfg = configparser.ConfigParser()
    cfg.read(CONFIG_PATH)
    section = 'SEQUENCERCONFIG'
    if not cfg.has_section(section):
        return jsonify({'error': f'Section [{section}] not found in config file'}), 404
    return jsonify(dict(cfg[section]))

@app.route("/api/config", methods=['POST'])
def config_post():
    data = request.json
    if not data:
        return jsonify({'error': 'No data provided'}), 400
    cfg = configparser.ConfigParser()
    cfg.read(CONFIG_PATH)
    section = 'SEQUENCERCONFIG'
    if not cfg.has_section(section):
        cfg.add_section(section)
    for key, value in data.items():
        cfg.set(section, key.upper(), str(value))
    try:
        with open(CONFIG_PATH, 'w') as f:
            cfg.write(f)
    except OSError as e:
        return jsonify({'error': str(e)}), 500
    return jsonify({'status': 'saved'})

@app.route("/api/register", methods=['POST'])
def register_user():
    data = request.json
    username = data.get('username')
    password = data.get('password')
    if not username or not password:
        return jsonify({'error': 'Username and password required'}), 400
        
    db_path = os.path.join(BASE_DIR, 'users.db')
    conn = sqlite3.connect(db_path)
    c = conn.cursor()
    try:
        c.execute("INSERT INTO users (username, password) VALUES (?, ?)", 
                  (username, generate_password_hash(password)))
        conn.commit()
    except sqlite3.IntegrityError:
        conn.close()
        return jsonify({'error': 'User already exists. Please login.'}), 400
    finally:
        conn.close()
        
    return jsonify({'status': 'success'})

@app.route("/api/login", methods=['POST'])
def login_user():
    data = request.json
    username = data.get('username')
    password = data.get('password')
    if not username or not password:
        return jsonify({'error': 'Username and password required'}), 400
        
    db_path = os.path.join(BASE_DIR, 'users.db')
    conn = sqlite3.connect(db_path)
    c = conn.cursor()
    c.execute("SELECT password FROM users WHERE username = ?", (username,))
    row = c.fetchone()
    conn.close()
    
    if row and check_password_hash(row[0], password):
        return jsonify({'status': 'success'})
    else:
        return jsonify({'error': 'Invalid credentials'}), 401

if __name__ == "__main__":
    serve(app, host='0.0.0.0', port=5041)
