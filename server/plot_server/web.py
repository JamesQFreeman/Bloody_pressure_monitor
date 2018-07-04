from flask import Flask, render_template, request, jsonify
from flask_bootstrap import Bootstrap
from flask_cors import CORS
import time
import json
from pylab import *
import matplotlib.pyplot as plt
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
        s = str(request.get_data(), encoding = 'utf-8')

        l = s.split(',')
        l = l[:1000]
        float_l =[]
        print('\n\n\n')
        print(l)
        print('\n\n\n')
        #data = {'sys': fuck[0], 'dia': fuck[1]}
        #db_str = s+','+time.strftime
        for item in l:
            float_l.append(float(item))
        plt.plot(float_l)
        plt.show()
        plt.close()
        #open('database.csv','a').write(s)
        return 'ok'
    if request.method == 'GET':
        return jsonify(data)

if __name__ == '__main__':
    data = {'sys': 'n/a', 'dia': 'n/a'}
    app.run("0.0.0.0", debug=True)
