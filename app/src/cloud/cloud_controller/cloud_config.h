#ifndef DEFAULT_CERTS_H
#define DEFAULT_CERTS_H

#define DEFAULT_CLOUD_ADDRESS "mqtts://sparkhub-LevelSense.azure-devices.net"

#define DEFAULT_DEVICE_ID "sparkhub-device-03"

#define DEFAULT_MQTT_USERNAME "sparkhub-LevelSense.azure-devices.net/" DEFAULT_DEVICE_ID "/?api-version=2021-04-12"

#define CLOUD_DEVICE_PROVISIONING_ADDRESS "mqtts://global.azure-devices-provisioning.net"
#define DEVICE_PROVISIONING_ID_SCOPE      "0ne00B73235"
#define DEVICE_PROVISIONING_MQTT_USERNAME \
    DEVICE_PROVISIONING_ID_SCOPE "/registrations/" DEFAULT_DEVICE_ID "/api-version=2019-03-31"

#define DEFAULT_TELEMETRY_MQTT_TOPIC "devices/" DEFAULT_DEVICE_ID "/messages/events/"

#define DEFAULT_SERVER_PUBLIC_CERT \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh\n" \
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
    "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\n" \
    "MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
    "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
    "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG\n" \
    "9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI\n" \
    "2/Ou8jqJkTx65qsGGmvPrC3oXgkkRLpimn7Wo6h+4FR1IAWsULecYxpsMNzaHxmx\n" \
    "1x7e/dfgy5SDN67sH0NO3Xss0r0upS/kqbitOtSZpLYl6ZtrAGCSYP9PIUkY92eQ\n" \
    "q2EGnI/yuum06ZIya7XzV+hdG82MHauVBJVJ8zUtluNJbd134/tJS7SsVQepj5Wz\n" \
    "tCO7TG1F8PapspUwtP1MVYwnSlcUfIKdzXOS0xZKBgyMUNGPHgm+F6HmIcr9g+UQ\n" \
    "vIOlCsRnKPZzFBQ9RnbDhxSJITRNrw9FDKZJobq7nMWxM4MphQIDAQABo0IwQDAP\n" \
    "BgNVHRMBAf8EBTADAQH/MA4GA1UdDwEB/wQEAwIBhjAdBgNVHQ4EFgQUTiJUIBiV\n" \
    "5uNu5g/6+rkS7QYXjzkwDQYJKoZIhvcNAQELBQADggEBAGBnKJRvDkhj6zHd6mcY\n" \
    "1Yl9PMWLSn/pvtsrF9+wX3N3KjITOYFnQoQj8kVnNeyIv/iPsGEMNKSuIEyExtv4\n" \
    "NeF22d+mQrvHRAiGfzZ0JFrabA0UWTW98kndth/Jsw1HKj2ZL7tcu7XUIOGZX1NG\n" \
    "Fdtom/DzMNU+MeKNhJ7jitralj41E6Vf8PlwUHBHQRFXGU7Aj64GxJUTFy8bJZ91\n" \
    "8rGOmaFvE7FBcf6IKshPECBV1/MUReXgRPTqh5Uykw7+U0b6LJ3/iyK5S9kJRaTe\n" \
    "pLiaWN0bfVKfjllDiIGknibVb63dDcY3fe0Dkhvld1927jyNxF1WW6LZZm6zNTfl\n" \
    "MrY=\n" \
    "-----END CERTIFICATE-----\n"

#define DEFAULT_CLIENT_PUBLIC_CERT \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIEhTCCAm2gAwIBAgIRAL0DiNvUgr0KjpHDuYuanA0wDQYJKoZIhvcNAQELBQAw\n" \
    "NDEyMDAGA1UEAwwpQXp1cmUgSW9UIEh1YiBJbnRlcm1lZGlhdGUgQ2VydCBUZXN0\n" \
    "IE9ubHkwHhcNMjMxMjA1MTI0MTM4WhcNMjQwMTA0MTI0MTM4WjAdMRswGQYDVQQD\n" \
    "DBJzcGFya2h1Yi1kZXZpY2UtMDMwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n" \
    "AoIBAQDAR4+eVstWRqEOrcRx4P+LcOkNHNpevW2jg7dVrAfs1zMvLyWWMKD2CnvV\n" \
    "cpJ+ot8nLDybHNgceB5sguva/0qUEO1SSnKrSzhfAleZaEwYFJ/Am5HyUDHgVVDH\n" \
    "4gY5XgoTdq3j5q9QBIoyiGmkI/tq3Dn506/bIHdpBg0eLmNxnJvUjQSccaTWZKUE\n" \
    "ZUBauYosF8mPVSgSpbDDWABTMZh+ROlP/Od0vEafmwHOrIZXMCnGhxUbznRmRFVc\n" \
    "7q8/6irreRJXgPo1c8iOgTf5XNSgWdlzC0pwH3o9j4xTzKf/ESyxDfc5X3S4vVmV\n" \
    "etdyuzV+7fnBDzOJB7/NDaIKZzElAgMBAAGjgagwgaUwCQYDVR0TBAIwADAzBglg\n" \
    "hkgBhvhCAQ0EJhYkT3BlblNTTCBHZW5lcmF0ZWQgQ2xpZW50IENlcnRpZmljYXRl\n" \
    "MB0GA1UdDgQWBBS19ClC23k0iJyeujVKaZ5DoxnbLDAfBgNVHSMEGDAWgBRrEbmf\n" \
    "hFeXWtw7uwnCuuJU8wi68DAOBgNVHQ8BAf8EBAMCBeAwEwYDVR0lBAwwCgYIKwYB\n" \
    "BQUHAwIwDQYJKoZIhvcNAQELBQADggIBADS1ubWw6xfzmOXsdykth89J7K0NSfUq\n" \
    "2AgMrzHB6UGMG5U1KBdy1gyYR56jcndEXq9f+K8u6f6iR8UiWMuXReyYZR2qhqG+\n" \
    "2DnsLL89GR/4Vda2AF2HPUTZy8o51J8tVwAat/m/Nv0NZ2D3ZzPy6pyARs4A9/7h\n" \
    "D+ael9RWlLGtJM2kMIupIM+imOwMn3780yW4naPSzXLC6CLvGAPLTxzfn5UFHW9z\n" \
    "6eW1fpKPjwqoVteeVQXcM68esoGqQ0IgJ2e9ZkOeOr23Y3mPUWD1r07s3LQhnzz5\n" \
    "HmACAkonVe4/7Y3vTTU2W76GrQT1wqpKUMBoDsP+h5qcYOuDH7r57PxGVnEOh+dr\n" \
    "EubseUxk5LZkhJUPwvos1QVrEVPriqagneSjsWbELATsit+hCyDVzyFWBaPLnxSe\n" \
    "AjSP0XjmwD7HjwExUz6ZGXK6i1h3Od252kqt7t7GBzfrYZGPNi3hIMUQASXPq6se\n" \
    "UlQ/Bncul7irx9FaY95lh/ZcSDSF40LK/R2zUz1zKuvj7yijogjKhGlEjbmrSHdz\n" \
    "mjEjQFz4vi2T661uLiC8hxFWCtSd/B737kY5buyBpWtUJCSQiFYx2r6OsWzDPsAo\n" \
    "EHbL67uiqI2ma4mF3Crdb+TMFuyQ2mAJ0/4cpYioLDMT4Q9fhlQshKTZvcwCPIB7\n" \
    "V6E3KoZWnZuX\n" \
    "-----END CERTIFICATE-----\n" \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIFTDCCAzSgAwIBAgIRAL0DiNvUgr0KjpHDuYuanAowDQYJKoZIhvcNAQELBQAw\n" \
    "KjEoMCYGA1UEAwwfQXp1cmUgSW9UIEh1YiBDQSBDZXJ0IFRlc3QgT25seTAeFw0y\n" \
    "MzExMjkwNTI5NDVaFw0yMzEyMjkwNTI5NDVaMDQxMjAwBgNVBAMMKUF6dXJlIElv\n" \
    "VCBIdWIgSW50ZXJtZWRpYXRlIENlcnQgVGVzdCBPbmx5MIICIjANBgkqhkiG9w0B\n" \
    "AQEFAAOCAg8AMIICCgKCAgEAmfKiuJoiPZqVdUq18womKjRo37m2xpyEjFK1lYCK\n" \
    "E0MMA/yrlO2dvC44kYhaChh3VUPWd1isezyocbvFjk1HjR7Y95Hf5wGPfijOH1Ra\n" \
    "1+9zpZ6Uxo77Fu9jEQgMn1lqin0+Z1tf/4g+WOCmrxS+BJZVJyXAPRkrS7unz4tr\n" \
    "dfZOpDKcOtt9HR0GemE53x9+hE+3rMh3hFws1r7iyXmbB+B1EzeKBjaHWxOnMw4C\n" \
    "H1Z+LCIE/lwRE9NElJYiBmNQoMbNr2QagQ0CJvn1fbmILYNDyL+0jMAIbuwGjnxI\n" \
    "lWRZeW50dMEe4m7uksKoPuiSwK/IS6MOT+8xUOwO92ntA0nddR5+mI/bmSYfRLUq\n" \
    "1RfBacOMETODB82nyQ4nwY/8EFNcsQqtov4rvaJ1FDzXvcN5Hnur62fpjZxwTLoI\n" \
    "In3fy7NONvWGdflDNlh7zX8sXc6/otCpUcKzXxP2Ov76mVnEbjcHi9aAZWdrR6A1\n" \
    "916OWTEvx0l/6UQ+FhMcaKrJaYrCCmcRX0GqOyQZ/jVH9O+INrUvT5UKqRYx0Ul5\n" \
    "Sv+/pPPklfOAAPzURaT4kxmB4k/VHhlq7m7QSxNaKYkRImsde9mfVsVtLTS3xdqA\n" \
    "+45lhKqBOsdgAVYUXPK7sc+SwM1cFULCHq/ZEEo+m7+Lv3APYsI1hbNf83vPVN4d\n" \
    "uV8CAwEAAaNjMGEwHQYDVR0OBBYEFGsRuZ+EV5da3Du7CcK64lTzCLrwMB8GA1Ud\n" \
    "IwQYMBaAFJh07R8oV+w4kCf6EY1HKPfdtpXVMA8GA1UdEwEB/wQFMAMBAf8wDgYD\n" \
    "VR0PAQH/BAQDAgGGMA0GCSqGSIb3DQEBCwUAA4ICAQABdOq7FoORqN/9O7LZCZfW\n" \
    "znNC3qE/llaNwNOLBulCc10oqZ4uKpX9wquwCv6AMjdfkjv59YFwwM7vtquYccVO\n" \
    "XU8Vb1kuOmgl/19p8g6IzadS4PZ2L4f/sI4vHekzNu+aJQp5cOd7Xyt19yXQMBXu\n" \
    "yQj06tLPp7CY/YzozSjJbTqTpW3COkx5Trqy4LgBmSZ/u8p/c5HnXfwQKlxrAuTd\n" \
    "su5JkFHSRtSP/1bYUQr/kkC36qSeH/LdDanJfxU9V+6S6dZmMzTnQhodXROdiJLs\n" \
    "53zSyu9QvGw5HEHg621qGuANPBXp8FqboKkpByBd8Qv8sV6NMqyN08WWWjZP4buw\n" \
    "ZFfB/M+fCedEcn8Pvm+YrZRcOA6jF6afAKrOV+nuop/L8MVCg3C7J9ecu2hPvJIo\n" \
    "iGVh48+QevyhlKHoRw/zqORlfydLhNCCs97GfcIp0azwJ+xk2w56sMgD4UV6teXw\n" \
    "cotGT3RoGTA6vQT1lNAAcT6rjlRohL1ZxQ88aTLbNspoi+49w7IxD6d4onDx8BeO\n" \
    "hZ8BGXI8QyniOjcoxw2HnPSjPgfNEnSWojIKs6C1FVok3hYQXQRNLG/pkNFQSrCd\n" \
    "E42uviNsSWVWjN9jyG2pnto1oYsPzJ7kJlFf2lDFWrSkJyK76x5XU3qMwRMT/26Q\n" \
    "hUjZpHQajcWC10X36n2/Mg==\n" \
    "-----END CERTIFICATE-----\n" \
    "-----BEGIN CERTIFICATE-----\n" \
    "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\n" \
    "SgTqseMBoM7h\n" \
    "-----END CERTIFICATE-----\n"

#define DEFAULT_CLIENT_PRIVATE_KEY \
    "-----BEGIN PRIVATE KEY-----\n" \
    "MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDAR4+eVstWRqEO\n" \
    "rcRx4P+LcOkNHNpevW2jg7dVrAfs1zMvLyWWMKD2CnvVcpJ+ot8nLDybHNgceB5s\n" \
    "guva/0qUEO1SSnKrSzhfAleZaEwYFJ/Am5HyUDHgVVDH4gY5XgoTdq3j5q9QBIoy\n" \
    "iGmkI/tq3Dn506/bIHdpBg0eLmNxnJvUjQSccaTWZKUEZUBauYosF8mPVSgSpbDD\n" \
    "WABTMZh+ROlP/Od0vEafmwHOrIZXMCnGhxUbznRmRFVc7q8/6irreRJXgPo1c8iO\n" \
    "gTf5XNSgWdlzC0pwH3o9j4xTzKf/ESyxDfc5X3S4vVmVetdyuzV+7fnBDzOJB7/N\n" \
    "DaIKZzElAgMBAAECggEAFVe8ItS6hEyZswHpxMQ3SViSndB+kraPFW/Y6UsLpCfs\n" \
    "+AdRMj8alsTqIvKxPRpCD9ySq5sDiAFzSxMrL+dXdWD9nr1Bxi62xqEBbTdM/v1H\n" \
    "ED+UbyOkraR6r30wuKxuDOQHAD2KZyTh3CMVKtGNpjpkuwzav8V2S7OOwDNg+Wq5\n" \
    "50Q/B3E8b/XYi+2ir287wyr80tf/QLblBHVg8XkY1oBAaqSCQpXtlGR/RBRQ/Buq\n" \
    "0KsTuiFYRnjT/43BmaiYVr15xFX6LR4EZIpewnxvogOmln/T7dOFLq9iUAvy82uk\n" \
    "tNizzGwNr0o5xEGAB49dS2RyUVKqMagU/LM+7B5oIQKBgQDO0PxMpFDTVH4Qwof0\n" \
    "Fep5LbEyjix2Yd+WtYDcFYeP88xfOuLpsJcsjTvHWRhY6NA4koyaxx45MHOArCzE\n" \
    "CohKtZRHHTO8aK4ShMP28d6bBdOCn+2wQXspTXuse/7bfj61sXuhuuPfDXaT76/3\n" \
    "e5Ts8Qs3NzL/lUDbEbva4B4FVQKBgQDuAZJcupYvnrZm3kiuOL21x/NonE6A/ulE\n" \
    "dwN8eJK+SJbgli+wsZMoPhy6hKfLX3dCXDk+1Rlwepf/asC3g+w2oPzdUqDEj+bl\n" \
    "cFkkNmn32aXv/XQxe3dt4CIyhxVKUS/txQ0unuq3JEAH6ennVf6BL5B1Fx6Aw4mR\n" \
    "P2t7I418kQKBgQCFy9HNS2g2r6y27wLBOARuzM6cJDpDg+IAP//TYeJHkLOg0BnI\n" \
    "kUPbwKrxtoP30PYUA0sPPs4gtWWj0r9LPH0byAjbUNsNgrNKq7WTZrpalBRXp1s/\n" \
    "8bzZ5ZSi67zfBvdO2DlhUeTlgnI6FpBCjSK1fuOAnydRDZ/dmcVsPh1t2QKBgQCh\n" \
    "Aw8xDEluDcD39ojU1C4DPDG/I384ZTsJxjnliuw7LoBjjzbUIk2be+mThQq+2uaj\n" \
    "TiCsER4UqdsKP/4AKI80pdRrJer9Dxz1OvwTM3yc3/ESWjSxBQXWnzqDVD9i92ml\n" \
    "QRmZ4IiJPUzUDbrfdZ2nZ7YigHFyStZWHMFpnLOrAQKBgG7e0BGLrmv0Ck0fUXpy\n" \
    "GJLXlcDGR7fZy2nzc2tQ30yv4REvJFc9D1yC8v1zVI/UDC+L1Zrz9vp7m2lY1WIV\n" \
    "pcl+3tj5CukvRpFMe5xoD4DeKT/MZJPP8fWZodGcKcrvOsjtQTP2c+5gDsUIIQmR\n" \
    "RY2qFiCtjOBOcZC2CpkSo9rO\n" \
    "-----END PRIVATE KEY-----\n"

#endif // DEFAULT_CERTS_H