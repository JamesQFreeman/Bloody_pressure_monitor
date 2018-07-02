from flask import Flask, render_template, request, jsonify
from flask_bootstrap import Bootstrap
from flask_cors import CORS

import json
app = Flask(__name__)
app.config['BOOTSTRAP_SERVE_LOCAL'] = True
Bootstrap(app)
CORS(app)


@app.route('/user/<name>')
def index(name):
    return render_template('home.html', title_name='welcome', name=name)


@app.route('/home/<name>')
def home(name):
    return render_template('home.html', title_name='welcome', name=name)


# @app.route('/home/<name>/temperature')
# def temperature(name):
#     return render_template('temperature.html', title_name='temperature', name=name)
#
#
# @app.route('/home/<name>/step')
# def step(name):
#     return render_template('step.html', title_name='step', name=name)
#
#
# @app.route('/home/<name>/fat')
# def fat(name):
#     return render_template('fat.html', title_name='fat', name=name)
#
#
# @app.route('/group19')
# def group():
#     return '<h1>Building</h1>'


@app.route('/data', methods=['GET', 'POST'])
def data_get():
    global data
    if request.method == 'POST':
        s = str(request.get_data(), encoding = 'utf-8');
        fuck = s.split(',')
        data = {'sys': fuck[0], 'dia': fuck[1]}
        return 'ok'
    if request.method == 'GET':
        return jsonify(data)


if __name__ == '__main__':
    data = {'sys': 'n/a', 'dia': 'n/a'}
    app.run("0.0.0.0", debug=True)
