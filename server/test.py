import unittest
import requests
import json
import time


class ProductTestCase(unittest.TestCase):

    def test_data_send(self):
        for i in range(0, 1000, 10):
            data = {'Temperature': str(30+i), 'Step': str(1999+i), 'Fat': str(9.1+i/100)}
            requests.post(url='http://127.0.0.1:5000/data', data=data)
            res = requests.get(url='http://127.0.0.1:5000/data')
            self.assertEqual(data, json.loads(res.text), 'Data transmission failed')

    def test_data_ajax(self):
        for i in range(0, 1000, 10):
            data = {'Temperature': str(30+i), 'Step': str(1999+i), 'Fat': str(9.1+i/100)}
            requests.post(url='http://127.0.0.1:5000/data', data=data)
            res = requests.get(url='http://127.0.0.1:5000/data_temperature')
            self.assertEqual({'Temperature': data['Temperature']}, json.loads(res.text), 'Data transmission failed')


if __name__ == '__main__':
    # unittest.main()
    for i in range(1, 100):
        data = {'Temperature': 30+i/10, 'Step': 1999, 'Fat': 9.1}
        requests.post(url='http://127.0.0.1:5000/data', data=data)
        time.sleep(1)
