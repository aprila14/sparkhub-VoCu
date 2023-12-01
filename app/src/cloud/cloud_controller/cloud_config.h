#ifndef DEFAULT_CERTS_H
#define DEFAULT_CERTS_H

#define CLOUD_DEVICE_PROVISIONING_ADDRESS "mqtts://global.azure-devices-provisioning.net"
#define CLOUD_ADDRESS "mqtts://sparkhub-LevelSense.azure-devices.net"

#define DEFAULT_DEVICE_ID "sparkhub-device-02"
#define DEFAULT_MQTT_USERNAME "sparkhub-LevelSense.azure-devices.net/" DEFAULT_DEVICE_ID "/?api-version=2021-04-12"

#define DEVICE_PROVISIONING_ID_SCOPE "0ne00B73235"
#define DEVICE_PROVISIONING_MQTT_USERNAME DEVICE_PROVISIONING_ID_SCOPE "/registrations/" DEFAULT_DEVICE_ID "/api-version=2019-03-31"

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
    "MIIEhTCCAm2gAwIBAgIRAL0DiNvUgr0KjpHDuYuanAwwDQYJKoZIhvcNAQELBQAw\n" \
    "NDEyMDAGA1UEAwwpQXp1cmUgSW9UIEh1YiBJbnRlcm1lZGlhdGUgQ2VydCBUZXN0\n" \
    "IE9ubHkwHhcNMjMxMjAxMDkwMTEyWhcNMjMxMjMxMDkwMTEyWjAdMRswGQYDVQQD\n" \
    "DBJzcGFya2h1Yi1kZXZpY2UtMDIwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n" \
    "AoIBAQDD4NYSqGNQuDhj+uBL823GDQ+IhOHGUirVJsOCqgZ2UzWDm90tSqokFPIP\n" \
    "k+BLBXJIWVcrX6eRSa4u+bFtqGsst5LVXFNskK2ZPss5neZM8MoV9vUkkeQLqPci\n" \
    "+WmeYr6ouCk4OXmrm3A6ig3h72hpJcP0ghqQ7DM+3wSuNzL/FMmJHAfvio6ct7zm\n" \
    "pt9U+WzWuLnPGXysvrPUZT9xcScaP2pVl/SUaKx8xiwHUehLNuaXlewc4mHdx6ay\n" \
    "qNJ17hadt9zrw1SK6hQUkYelDuyCelV6lpZTSRdDlm/zndNJ0yBdnDeJZL8HIdYG\n" \
    "+raDMCHgoxrFDIu9HnGvQVGoN2srAgMBAAGjgagwgaUwCQYDVR0TBAIwADAzBglg\n" \
    "hkgBhvhCAQ0EJhYkT3BlblNTTCBHZW5lcmF0ZWQgQ2xpZW50IENlcnRpZmljYXRl\n" \
    "MB0GA1UdDgQWBBQIIsDIW1lQN28siP9N3PcbU6gFEjAfBgNVHSMEGDAWgBRrEbmf\n" \
    "hFeXWtw7uwnCuuJU8wi68DAOBgNVHQ8BAf8EBAMCBeAwEwYDVR0lBAwwCgYIKwYB\n" \
    "BQUHAwIwDQYJKoZIhvcNAQELBQADggIBABc7riLOYOkHA65zT3Q7YlqfIJipQiU8\n" \
    "l5WDsZ87jkjqOP8WNbURerivzi6uquIYe04ErEoWdm69UG7v10/U4N0O6TMxr5Gy\n" \
    "JlBy3tsp3YfdsVcToVWbJchnl4PExL4SogdhPBLt9Pvn/miraChRUYIjF0FTvCBV\n" \
    "xqOU7gu0HBVBDQNPzZtXNsaqJ/xSUfOdnVxfft/lHMHtlHgM9fjugkgFTp9YL/pg\n" \
    "Qb4SlbKmDYAwapgfhpAPIi4UON2TU/pfLX4JVhlTDUIQvPyDG1l+7Z15evOsjLfP\n" \
    "E2qeT6g3W4hk+9aOlCrq+LiL0H5Kha6IC/8Ls4IqK923GVToyfwt/plB+ppvKwvR\n" \
    "jdywio8n3XSx7gZ4SqAO7M4cYJtvHMwAaX49s9sChBdwoxy/0IzR+dCCaVB2AbgK\n" \
    "QB2xKMNxWtz9FJtLGM7dfEg/mQrBF+xHRe0lkXSwFRkkAQEiHXiyk8U4ZB/pYjIh\n" \
    "Voyi5PIx1AGM4NDNnmzMfRpEJpwh7391MuLvrTpNQbQbvhONZdpDCbdVufmHXpzl\n" \
    "Et1Ljckf0R6MFkDMBWEPOOhOnh3mM7Yj+KS4U5AU0KW2n0ebiix5IFBna+fiYTXb\n" \
    "stD6R/UYEvtX/Ykx28aSacwti6nw4ECrWryPz+SMOLmTmixUBmQ7CCQZP+ME6c4t\n" \
    "s9Id0LtPF2Bz\n"                                                     \
    "-----END CERTIFICATE-----\n"

#define DEFAULT_DEVICE_PROVISIONING_INTERMEDIATE_PUBLIC_CERT             \
    "-----BEGIN CERTIFICATE-----\n"                                      \
    "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\n" \
    "hUjZpHQajcWC10X36n2/Mg==\n"                                         \
    "-----END CERTIFICATE-----\n"

#define DEFAULT_DEVICE_PROVISIONING_ROOT_CERT                            \
    "-----BEGIN CERTIFICATE-----\n"                                      \
    "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\n" \
    "SgTqseMBoM7h\n"                                                     \
    "-----END CERTIFICATE-----\n"

#define DEFAULT_CLIENT_PRIVATE_KEY                                       \
    "-----BEGIN PRIVATE KEY-----\n"                                      \
    "MIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQDD4NYSqGNQuDhj\n" \
    "+uBL823GDQ+IhOHGUirVJsOCqgZ2UzWDm90tSqokFPIPk+BLBXJIWVcrX6eRSa4u\n" \
    "+bFtqGsst5LVXFNskK2ZPss5neZM8MoV9vUkkeQLqPci+WmeYr6ouCk4OXmrm3A6\n" \
    "ig3h72hpJcP0ghqQ7DM+3wSuNzL/FMmJHAfvio6ct7zmpt9U+WzWuLnPGXysvrPU\n" \
    "ZT9xcScaP2pVl/SUaKx8xiwHUehLNuaXlewc4mHdx6ayqNJ17hadt9zrw1SK6hQU\n" \
    "kYelDuyCelV6lpZTSRdDlm/zndNJ0yBdnDeJZL8HIdYG+raDMCHgoxrFDIu9HnGv\n" \
    "QVGoN2srAgMBAAECggEAFe+6mblQ8gvHiO7iKq9W5ly1FiFnlYnDMT7Xm20GLz8v\n" \
    "WY9djEJ13neYCkWR02UHQPH8cKeUQRGi17Dv/WtkoUvnvkxKLXjgcC6XnNrBnXFD\n" \
    "+ITBPHi4NNQV5U1QVkQRvRxuxVSuexj7zxqB5oEkzM3Fbb8aUYuaS5q3T/lvlSBg\n" \
    "y8QOgB/EjJv8JogSsVp92yfu4n3ZMj28GzxfB2PBb+wxrOqlXyyd/bTAkat3a3kR\n" \
    "n4WjxoXRGXNp4wenmM7QRaCPlUmGejvei1n4xcbUy4WYk250Q2WPdaiaPTkt7Vjz\n" \
    "1veRF9nhEGkMZT7xDcpolELJjvKSKS0axjnH7i0BcQKBgQD9ojJe0q7SOluzYWLM\n" \
    "1JsRBDWVsqgTzuaSed2L9/c/CkurJp7TEgYAM5Yg9JzmoG1PIc9dlbVMTdGi1A3O\n" \
    "ASjwILKXcVZ0Tl+sK+0xSKP9Cag/7E1UjG3bfGpKaO6rQbS1Wx97Oo+5Rk7uplJE\n" \
    "rS/rVcxq+bV4Dk1kI1Dy9dk3uwKBgQDFtLDm5Vr0KUcQvXeDGIrkkDIdLDNVpQuD\n" \
    "dDhFR77D4sLFr0jbINO/Vh6ceNnn+vWAfRLhd2hA5CAaa9mEe80v9dUc0FAbu3kz\n" \
    "Ud+lY88Awo9s8jK8OjW76yqLteumaucKNCjHJBaDHlCraLQVhL/vJ64cQxzb2WVC\n" \
    "PQn70GtLUQKBgCuL0YIeQg2uH1ho/AGW7xi41EpjZiAggc3cRoFljXmxVtSHyPUM\n" \
    "PyOBt+DonpeqK5pOf7/dmEcFhVyjc8tOv6yasI730pQdcKeq0wqHIkCri8uN0qkV\n" \
    "gzcP4V/tLC0TFuecQYdywoJY4+xteep4oaWSEYq8XL/EFN2AgsF0HFtJAoGAfSnB\n" \
    "fMvi4wRm2zA3wyT8eWrYbyOWdDgCq/wdy656FOUwTZqE00VbCJzq0ECqI3T7i5aj\n" \
    "J15DACHsOQ3iLT97TUui2ZT/wi7Tsiu5HKmtWVvTFTPX7tYjQo7E5qILR9YR4qHW\n" \
    "1wv/vI6qn3mv2JesMym5zmamlIBy3JZsBaGS66ECgYBRcH8VaH6woTXmbW1PAgr8\n" \
    "cDzs6kW98x3/11VwK0eMFd9GCDCa9cNGoeSgZ5agNKwgle2W+I7M1SLqtRGio6JN\n" \
    "OMFSvext8jRig9q0arJhW+1IR+a7ctx12JQ30NEunFzuu5BQlx/IVj8f2NHSLMpO\n" \
    "/odz1lT5Op0gZouPb5bTsA==\n"                                         \
    "-----END PRIVATE KEY-----\n"

#endif // DEFAULT_CERTS_H