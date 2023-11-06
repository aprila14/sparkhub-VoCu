# ThingsBoard on-premises installation

Instruction based on the [Thingsboard guide](https://thingsboard.io/docs/user-guide/install/ubuntu/), for more detailed instruction refer to the original link.

</br>

## Step 1. Install Java 11 (OpenJDK)

```
sudo apt update
sudo apt install openjdk-11-jdk
```

Configure your operating system to use OpenJDK 11 by default:

```
sudo update-alternatives --config java
```

## Step 2. ThingsBoard service installation

Download installation package:

```
wget https://github.com/thingsboard/thingsboard/releases/download/v3.3.4.1/thingsboard-3.3.4.1.deb
```

Install Thingsboard as a service

```
sudo dpkg -i thingsboard-3.3.4.1.deb
```

## Step 3. Configure ThingsBoard database

PostgreSQL was selected as default, for other options refer to Thingsboard guide

```
# install **wget** if not already installed:
sudo apt install -y wget

# import the repository signing key:
wget --quiet -O - https://www.postgresql.org/media/keys/ACCC4CF8.asc | sudo apt-key add -

# add repository contents to your system:
RELEASE=$(lsb_release -cs)
echo "deb http://apt.postgresql.org/pub/repos/apt/ ${RELEASE}"-pgdg main | sudo tee  /etc/apt/sources.list.d/pgdg.list

# install and launch the postgresql service:
sudo apt update
sudo apt -y install postgresql-12
sudo service postgresql start
```

Create new user and set password:

```
sudo su - postgres
psql
\password
\q
```

Press Ctrl+D to return to main user console and connect to the database to create thingsboar DB:

```
psql -U postgres -d postgres -h 127.0.0.1 -W
CREATE DATABASE thingsboard;
\q
```
</br>

### Thingsboard Configuration

</br>

To change Thingsboard Configuration you need to edit configuration file with following command:

```
sudo nano /etc/thingsboard/conf/thingsboard.conf
```

Add following lines to configuration file. Replace "PUT_YOUR_POSTGRESQL_PASSWORD_HERE" with postgres user password set in previous step

```
# DB Configuration 
export DATABASE_TS_TYPE=sql
export SPRING_JPA_DATABASE_PLATFORM=org.hibernate.dialect.PostgreSQLDialect
export SPRING_DRIVER_CLASS_NAME=org.postgresql.Driver
export SPRING_DATASOURCE_URL=jdbc:postgresql://localhost:5432/thingsboard
export SPRING_DATASOURCE_USERNAME=postgres
export SPRING_DATASOURCE_PASSWORD=PUT_YOUR_POSTGRESQL_PASSWORD_HERE
# Specify partitioning size for timestamp key-value storage. Allowed values: DAYS, MONTHS, YEARS, INDEFINITE.
export SQL_POSTGRES_TS_KV_PARTITIONING=MONTHS
```

## Step 4. Choose Thingsboard queue service

**In Memory** queue implementation was selected as default, which does not require any additional configuration. For other queue implementation options refer to Thingsboard guide.

## Step 5. [Optional] Memory update for slow machines (1GB of RAM)

In case of slow machines you may edit Thingsboard configuration file:

```
sudo nano /etc/thingsboard/conf/thingsboard.conf
```

and add following line

```
# Update ThingsBoard memory usage and restrict it to 256MB in /etc/thingsboard/conf/thingsboard.conf
export JAVA_OPTS="$JAVA_OPTS -Xms256M -Xmx256M"
```

## Step 6. Run installation script

Now you should be able to run installation script with following command:

```
# --loadDemo option will load demo data: users, devices, assets, rules, widgets.
sudo /usr/share/thingsboard/bin/install/install.sh --loadDemo
```

## Step 7. Start ThingsBoard service

Execute following command to start ThingsBoard:

```
sudo service thingsboard start
```

Now you should be able to connect to Thingsboard using Web UI under following link:

```
http://localhost:8080
```

Following default credentials are available:

- System Administrator: sysadmin@thingsboard.org /sysadmin
- Tenant Administrator: tenant@thingsboard.org / tenant
- Customer User: cusomer@thingsboard.org / customer

You can change passwords for each account in account profile page.

Thingsboard logs are stored in the following directory:

```
/var/log/thingsboard
```

To check if there are any errors on backend side you can issue following command:

```
cat /var/log/thingsboard/thingsboard.log | grep ERROR
```

# X.509 Certificate Based Authentication

For detailed information on X.509 certificate authentication please refer to [Thingsboard Guide](https://thingsboard.io/docs/user-guide/certificates/).

</br>

## Step 8 - Configuration of Thingsboard for SSL

Follow [Thingsboard MQTT/HTTP over SSL Guide](https://thingsboard.io/docs/user-guide/mqtt-over-ssl/) for details

Add following lines to thingsboard.conf file:

```
...
export MQTT_SSL_ENABLED=true
export MQTT_SSL_CREDENTIALS_TYPE=PEM
export MQTT_SSL_PEM_CERT=server.pem
export MQTT_SSL_PEM_KEY=server_key.pem
export MQTT_SSL_PEM_KEY_PASSWORD=secret

export SSL_ENABLED=true
export SSL_CREDENTIALS_TYPE=PEM
export SSL_PEM_CERT=server.pem
export SSL_PEM_KEY=server_key.pem
export SSL_PEM_KEY_PASSWORD=secret
...
```

where:

- MQTT_SSL_ENABLED - Enable/disable SSL support;
- MQTT_SSL_CREDENTIALS_TYPE - Server credentials type. PEM - pem certificate file; KEYSTORE - java keystore;
- MQTT_SSL_PEM_CERT - Path to the server certificate file. Holds server certificate or certificate chain, may also include server private key;
- MQTT_SSL_PEM_KEY - Path to the server certificate private key file. Optional by default. Required if the private key is not present in server certificate file;
- MQTT_SSL_PEM_KEY_PASSWORD - Optional server certificate private key password.

Parameters without 'MQTT' prefix refer to the same values for HTTPS protocol.

After completing the setup, start or restart the ThingsBoard server.

Note: make sure that certificate and key are reachable by ThingsBoard process - place them in /etc/thingsboard/conf folder. Make sure the files have the same permisions as thingsboard.conf and user relative path, e.g. server.pem.


## Step 9 - Self-signed server certificates generation

To generate self-signed PEM certificate and private key, use the following command:

```
openssl ecparam -out server_key.pem -name secp256r1 -genkey
openssl req -new -key server_key.pem -x509 -nodes -days 365 -out server.pem 
```

The **days** parameter can be replaced with any number to affect the expiration date. You will get prompts about 'Country Name', etc., you can leave them as default by hitting Enter. Note: when you are asked about 'Common Name', you need to type your local address, e.g. '192.168.1.37'. 


## Step 10 - Client certificate generation

To generate RSA based key and certificate, use:

```
openssl req -x509 -newkey rsa:4096 -keyout key.pem -out cert.pem -sha256 -days 365 -nodes
```

## Step 11 - Provision Client Public Key as Device Credentials

Go to **ThingsBoard Web UI** -> **Devices** -> **Your Device** -> **Details** -> **Manage Credentials**. Keep in mind, that from the moment SSL was enabled in the config file, you need to connect to **Thingsboard Web UI** with the address starting from http**s**.

Select X.509 Certificate device credentials, insert contents of cert.pem file (client) and click save.


## Step 12 - testing the connection with mosquitto_pub

Execute the following command to make sure the connection is working correctly before testing this on ESP32

```
mosquitto_pub --cafile tb-server-chain.pem -d -q 1 -h "YOUR_TB_HOST" -p "8883" -t "v1/devices/me/telemetry" --key key.pem --cert cert.pem -m {"testUploadTelemetry":25}
```

where **YOUR_TB_HOST** is address of the computer on which ThingsBoard service is running, e.g. 192.168.1.37. After executing the command, sent message should appear under **Devices** -> **Your Device** -> **Latest telemetry**. If the sent message does not appear in the **Latest telemetry tab** you can make sure if default Rule chain is selected **Rule chains** -> **Root Rule Chain** -> **Make rule chain root**.

To connect ESP32 to Thingsboard cloud you need to place server public certificate, client public certificate and client private key in the PC BLE client build folder and change their names accordingly ("home_server.pem", "client_public_cert.pem", "client_private_key.pem").


# Setting up rule chain and dashboard


## Step 13 - import the rule chain

Go to **Rule chains** -> **Add Rule Chain** ('+' sign) -> **Import rule chain**

Select [i2a_rule_chain.json](./i2a_rule_chain.json) file from $PROJECT_DIR/misc/thingsboard. Click on the newly imported rule chain and select **Make rule chain root**


## Step 14 - import the dashboard

Go to **Dashboards** -> **Add Dashboard** ('+' sign) -> **Import dashboard**

Select [i2a.json](./i2a.json) file from $PROJECT_DIR/misc/thingsboard. Click on the newly imported dashboard and click **Open dashboard**. Right now you should be able to connect to the ThingsBoard cloud using ESP32 and test it's features.