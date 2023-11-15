#ifndef DEFAULT_CERTS_H
#define DEFAULT_CERTS_H

#define CLOUD_ADDRESS "mqtts://pressure-sensor-paid.azure-devices.net"

#define DEFAULT_DEVICE_ID "sparkhub-pressure-sensor-3"
#define DEFAULT_MQTT_USERNAME "pressure-sensor-paid.azure-devices.net/" DEFAULT_DEVICE_ID "/?api-version=2021-04-12"

#define DEFAULT_TELEMETRY_MQTT_TOPIC "devices/" DEFAULT_DEVICE_ID "/messages/events/"

#define DEFAULT_SERVER_PUBLIC_CERT                                       \
    "-----BEGIN CERTIFICATE-----\n"                                      \
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
    "MrY=\n"                                                             \
    "-----END CERTIFICATE-----\n"

#define DEFAULT_CLIENT_PUBLIC_CERT                                       \
    "-----BEGIN CERTIFICATE-----\n"                                      \
    "MIIDaDCCAlCgAwIBAgIQFiSFPcSv85PIRj35A4kFLzANBgkqhkiG9w0BAQsFADAT\n"\
    "MREwDwYDVQQDDAhzcGFya2h1YjAeFw0yMzEwMTgxNDM3NDVaFw0zMzEwMTUxNDM3\n"\
    "NDVaMFYxCzAJBgNVBAYTAkNIMQ8wDQYDVQQIDAZ6dXJpY2gxETAPBgNVBAoMCHNw\n"\
    "YXJraHViMSMwIQYDVQQDDBpzcGFya2h1Yi1wcmVzc3VyZS1zZW5zb3ItMzCCASIw\n"\
    "DQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKbM5DXbh+ySfoR0gH0fizFz8U8U\n"\
    "sH8B9h+43FJkApfD1yby349WLdwOYg2oeN5pzkIS+mvSgJu7MYNVFqkreIADjmHJ\n"\
    "Uzyc5vZeLQGHUJQfq1bpLCYKwaiR/q97OIwAyvGKdtG2JY5yvGE+mZMBYK0HuGvY\n"\
    "wzFryP8XXjI1wB0vQGQw/wQYzoHiSIHmmLPCQEZjOXmlBxLe8HBrahW7mNHrkT30\n"\
    "nutjBypTJ7TV53q7Fu3Hx6WiLZQWpd/Opj6BvHRLmjcQs5GdwHbXW5suwhAeNfPU\n"\
    "GoYOkK7au8nik1a4qVZLCxnYqjHPEyE8PrxPrQXLt3e8LTeXqU0sK/15ZfcCAwEA\n"\
    "AaN1MHMwHwYDVR0jBBgwFoAUJbQ7Umg1bHofPHzZFuT1hUicxZQwDAYDVR0TAQH/\n"\
    "BAIwADATBgNVHSUEDDAKBggrBgEFBQcDAjAOBgNVHQ8BAf8EBAMCB4AwHQYDVR0O\n"\
    "BBYEFDB3w2V0DT4d2qSx4C790M31EnasMA0GCSqGSIb3DQEBCwUAA4IBAQA72tVh\n"\
    "7dbr3VfoE3qRRr3HX8hn4kJLTXDXs4W9qD3Wat2fKUfD/a5enFFSH6S3pKQATDc2\n"\
    "O6G85m1k/jt6ze/BghFJz/mfq3eTa5jBxTnhSO4PW7sGRFHWDr8NgXYSo9dxE2tu\n"\
    "McpcO3rnj9FajR8rnFMpG9ooe58pSAYG4jWNwv8j95Mie8Za6aD+nX+wWGpLtnJx\n"\
    "ZzKTxYUSVNtZlJyPTc+qGP1h/+d31U6f3OqflR5xfWLik2OFsARpJ5E/PySAq5b7\n"\
    "IVVra8vxZTbFRwXaY8VtrNoVoQwrBZzK3Fo7kG8qlUNnOraHMcBfDVCYQYcxAXEq\n"\
    "OlxJhE9JfRtewUTl\n"\
    "-----END CERTIFICATE-----\n"

#define DEFAULT_CLIENT_PRIVATE_KEY                                       \
    "-----BEGIN PRIVATE KEY-----\n"                                      \
"MIIEvwIBADANBgkqhkiG9w0BAQEFAASCBKkwggSlAgEAAoIBAQCmzOQ124fskn6E\n"\
"dIB9H4sxc/FPFLB/AfYfuNxSZAKXw9cm8t+PVi3cDmINqHjeac5CEvpr0oCbuzGD\n"\
"VRapK3iAA45hyVM8nOb2Xi0Bh1CUH6tW6SwmCsGokf6veziMAMrxinbRtiWOcrxh\n"\
"PpmTAWCtB7hr2MMxa8j/F14yNcAdL0BkMP8EGM6B4kiB5pizwkBGYzl5pQcS3vBw\n"\
"a2oVu5jR65E99J7rYwcqUye01ed6uxbtx8eloi2UFqXfzqY+gbx0S5o3ELORncB2\n"\
"11ubLsIQHjXz1BqGDpCu2rvJ4pNWuKlWSwsZ2KoxzxMhPD68T60Fy7d3vC03l6lN\n"\
"LCv9eWX3AgMBAAECggEAJdnpD6+5sJ2tyDEH110JITfGUsPHKYswZxJ0LN41GGuA\n"\
"r3397XEJBx7ccW0BwLcR7zyPR1iFhtF78umkMs+CkMo9zUL6eI7CerX0UJx8VKC7\n"\
"KQXQ1vEp8S7wNLIpqtujMhzRydUjwKeYyBiEkS+G2L0fxJlz8gsbmYecAJjh970b\n"\
"WV83Cua6/BqTBrrYWMZe/+yLEVSpYjFoEMkXVAkZzdi9VQu317szyqPdkb+C6mtV\n"\
"pE8Kn0K3jnWvEIf3rVV9YtBEvkIhfQs8PYwcY3Pvo+47ZlRNqUL1JbX08ADThtkZ\n"\
"hRCRbDoBiyqhXxmDMLyl95PBseaqS57mES8GzeeFQQKBgQDacdc6AqRHaAay5JSS\n"\
"OudVgSNuy+38sNjPOz/AyKdb/T4XbpQUHbiQcHWi91lzAc/JbF/kz9JIizw6lzRA\n"\
"Tp3Bb1QrlsxXmekveweq354PgvaVPu70q2UEiygLNc5MSdHc1RtlMkOoYVuUEef1\n"\
"5fcQo0iEsMB0qGrjLD25O2x44wKBgQDDehZJCBqAt7axjAF7UwHkdxuF3GcfeNXk\n"\
"lAghbqwqzyAh2jzt5OLdFJ7+ynsio+oP9LmQykpWfHAmmQqggnYK2cQ2VCI2P+vD\n"\
"7BtCTEqM9ZMNR7KKSRgEWPAP6GUweMkMKFQHdRZajBNveBNiaY8qbVyeOu12+IBV\n"\
"e1F1+mbu3QKBgQCcFo5Q8ERMlcN7nqOe3aIhUdZsvCFA12kEovSKH5aev5TXj3cV\n"\
"7KrN4t3b5dyvthKwfdUDnZBHr94Ii7SVYteMua0uPGGlEuAxwjqXWZQKx2zE9iQv\n"\
"Bi13kqu5XugbUQA5SLl7lPANaAvVAPp7laf15obR8a6DbTh7PUiZjCGVOwKBgQCc\n"\
"Zo52GHm8C0GthOualKuq9HGdBN+tmqzomlh3B2nRRq/a5hZh8QRjByC/k5o5+RRJ\n"\
"d/ERHAmmcSH6f/C0xC1WOS54mZBV/4bP15Vn0krDxajGs1rysdD5iX9p+vMbeUjC\n"\
"5XF2FgSwperXASfhUbwuohm8sEFXMzTbMDPZRWQeMQKBgQDQXBExQblnB9Ky2TQz\n"\
"zZ2iUbZdgLf5LGSbYtK1KQNOzJ5aUWrUUy4Fj5GTfHL++S7PmhodESuL6RaCAjy7\n"\
"YjQaAWrSNbjqsuqaOFacHN+asoz3JgiRqFCxOHuzm42pkN1F7nveoVpMvqqEEll8\n"\
"kjUUIFwEV+ySIIE3fp6SqLBrNQ==\n"\
    "-----END PRIVATE KEY-----\n"

#endif // DEFAULT_CERTS_H