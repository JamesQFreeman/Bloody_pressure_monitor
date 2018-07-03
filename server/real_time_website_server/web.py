from flask import Flask, render_template, request, jsonify
from flask_bootstrap import Bootstrap
import json
app = Flask(__name__)
app.config['BOOTSTRAP_SERVE_LOCAL'] = True
Bootstrap(app)


@app.route('/user/<name>')
def index(name):
    return render_template('home.html', title_name='welcome', name=name)


@app.route('/home/<name>')
def home(name):
    return render_template('home.html', title_name='welcome', name=name)


@app.route('/home/<name>/temperature')
def temperature(name):
    return render_template('temperature.html', title_name='temperature', name=name)


@app.route('/home/<name>/step')
def step(name):
    return render_template('step.html', title_name='step', name=name)


@app.route('/home/<name>/fat')
def fat(name):
    return render_template('fat.html', title_name='fat', name=name)


@app.route('/group19')
def group():
    return '<h1>Building</h1>'


@app.route('/data', methods=['GET', 'POST'])
def data_get():
    global data
    if request.method == 'POST':
        s = str(request.get_data(), encoding = 'utf-8')
        print(s[12:16])
        data = {'Temperature': s[12:16], 'Step': '999', 'Fat': '10.0'}
    if request.method == 'GET':
        return json.dumps(data)


@app.route('/data_temperature', methods=['POST', 'GET'])
def data_temperature():
    return jsonify({'Temperature': data['Temperature']})


@app.route('/data_step', methods=['POST', 'GET'])
def data_step():
    return jsonify({'Step': '1234'})


if __name__ == '__main__':
    data = {'Temperature': 36.8, 'Step': 1999, 'Fat': 9.1}
    app.run("0.0.0.0", debug=True)
