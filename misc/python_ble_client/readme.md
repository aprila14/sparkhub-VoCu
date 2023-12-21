# Python BLE client

### Tested environments
* Ubuntu 22.04
* bluez 5.64-0ubuntu1
* Python 3.10.12

* Windows 10
* Python 3.10.11

## Installation

Please install Python 3.10 with pip

### Prerequisites
```
pip install pipenv==2023.8.28
```
### Pipenv and Pipfile
```
cd misc/python_ble_client
pipenv install
```

## Configuration

Names, expiration time and other certificate related information can be set in the file configuration.py

Every directory/file path is relative and assumes as origin the misc/python_ble_client.

In order for the application to work correctly and generate valid device certificates the user has to:
- place the root CA and intermediate certificate in the `CERTS_DIRECTORY` (by default "certs")
- place the intermediate private key in the `CERTS_PRIVATE_DIRECTORY` (by default "private")

## Start
```
cd misc/python_ble_client
pipenv run python main.py
```
To Stop Ctrl+C
