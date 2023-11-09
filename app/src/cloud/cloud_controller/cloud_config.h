#ifndef DEFAULT_CERTS_H
#define DEFAULT_CERTS_H

#define CLOUD_ADDRESS "mqtts://pressure-sensor-paid.azure-devices.net"

#define DEFAULT_DEVICE_ID "sparkhub-pressure-sensor-2"
#define DEFAULT_MQTT_USERNAME "pressure-sensor-paid.azure-devices.net/" DEFAULT_DEVICE_ID "/?api-version=2021-04-12"

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
    "MIIDaDCCAlCgAwIBAgIQFiSFPcSv85PIRj35A4kFLjANBgkqhkiG9w0BAQsFADAT\n" \
    "MREwDwYDVQQDDAhzcGFya2h1YjAeFw0yMzA5MjkwNjUyMDZaFw0zMzA5MjYwNjUy\n" \
    "MDZaMFYxCzAJBgNVBAYTAkNIMQ8wDQYDVQQIDAZadXJpY2gxETAPBgNVBAoMCHNw\n" \
    "YXJraHViMSMwIQYDVQQDDBpzcGFya2h1Yi1wcmVzc3VyZS1zZW5zb3ItMjCCASIw\n" \
    "DQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMOx0etSieNB5fazYfTIaVd5pw4L\n" \
    "c5gnVVUxSENXKYMhJVykhR+P18C8YTeDQbVH6r0HdVytj6tAl8LfN54qeRtw46ef\n" \
    "PAi+8Iw9fsOs4gbtwZ0zIaC/59cDEPz7fRHE4u6ZvBLjlbUrx5l+eXtj5k0c0Szi\n" \
    "LIhizBQuwRwXlbXDTOnZpppYMzMC4/uTZPVw2DW/lmqoCrQerQG4AxkbBCKSCjdy\n" \
    "DpvyfBF52P2wZCkJ4+OvY4kqavVncA508V2b+GGD2T9jmw2BU6YaTc+rd7iBwemF\n" \
    "Dy6QVePQ7XPgExz7INSi/RhTX3XB9MCO+oD1QG6bz32RCmEdEk6dCSSEjgUCAwEA\n" \
    "AaN1MHMwHwYDVR0jBBgwFoAUJbQ7Umg1bHofPHzZFuT1hUicxZQwDAYDVR0TAQH/\n" \
    "BAIwADATBgNVHSUEDDAKBggrBgEFBQcDAjAOBgNVHQ8BAf8EBAMCB4AwHQYDVR0O\n" \
    "BBYEFJgGnFhY5IHpdnp7ol2e2yIvpWV+MA0GCSqGSIb3DQEBCwUAA4IBAQAvnv1Q\n" \
    "TsSvpnefCFQAef84AcDyLuGkBww99AXurm33iTAXIuAME3FEwaHKrF15XxQbBLdN\n" \
    "RXX19DuF2vngg7fj4MtC1Qc3yTanJUraWrJE5YbYVHXUkLrhI7bZ2O11hFl/V3Pn\n" \
    "78IWxhSejEHetC/x+ZPtIAme2vzImGqXLAerQN506AMbj0rOkwmezg18R9dm9akL\n" \
    "n8OiVkrStpJIU6AaWwkYVEoQ57m3CMI7pS1D/ISrWwnOuyNOyTJsCTToBpCsUZPu\n" \
    "afSqPAgAdIs5M9rJeTPZ/nuHHRkiiZ86DHIS9Z5xHBxGGfg6hlUMT9cBo0PMb9UE\n" \
    "KZC3lUePBMEgnB5y\n"                                                 \
    "-----END CERTIFICATE-----\n"

#define DEFAULT_CLIENT_PRIVATE_KEY                                       \
    "-----BEGIN PRIVATE KEY-----\n"                                      \
    "MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDDsdHrUonjQeX2\n" \
    "s2H0yGlXeacOC3OYJ1VVMUhDVymDISVcpIUfj9fAvGE3g0G1R+q9B3VcrY+rQJfC\n" \
    "3zeeKnkbcOOnnzwIvvCMPX7DrOIG7cGdMyGgv+fXAxD8+30RxOLumbwS45W1K8eZ\n" \
    "fnl7Y+ZNHNEs4iyIYswULsEcF5W1w0zp2aaaWDMzAuP7k2T1cNg1v5ZqqAq0Hq0B\n" \
    "uAMZGwQikgo3cg6b8nwRedj9sGQpCePjr2OJKmr1Z3AOdPFdm/hhg9k/Y5sNgVOm\n" \
    "Gk3Pq3e4gcHphQ8ukFXj0O1z4BMc+yDUov0YU191wfTAjvqA9UBum899kQphHRJO\n" \
    "nQkkhI4FAgMBAAECggEAInn/e6/s7wa7Fv1/cnqkZG7gSY8m7FVxqTKngoWmoJ2E\n" \
    "UZACjA7xedk2X2tWdjOXXXok6REC0K7ujbx5tfnRoE4BLZTWJMxO4MYswRiCMapN\n" \
    "g/5wHDvzAmRd59P+stS3DkYzZjiGNYbjAV3Q1TZCaq97AqdOJTEStzONff+BtqXE\n" \
    "mlxlW1etOtFwmELdhHQJl6YuXXuN1KeSo3v2JBIIbcxAoXWYjjN/8KbR77ysL9rE\n" \
    "WaspIB/M3bSXyBQKs2XTXPIsGCva6FX6izB4uNnIEmrMtBPT4V3ObBD7SKShFjxg\n" \
    "PKmTk7g0VX0wE8AGQUeDlavoW0DQIIzRPr3j4OV+gQKBgQDPFbHcOXY1zZBHipKD\n" \
    "dozZAYvLsN+u0PNLqnilD4FQbk88Ml4txy0urOriUO+kko3Q/ymrFRAkXuavDM3T\n" \
    "Sv0pcNfLyQ2B+zsRXCy4B4/UVBlRmBEns5JUbS6SRWXT029KWHs6c65PjbjdYeaf\n" \
    "xSBZ+Vpgul22i0VP90y6v/kLdQKBgQDx616mmS1c0HrqoI11FrxOw8ACKeMRyIge\n" \
    "tk78H9t/eGGGob3C291VdcqjVuOEHEAdnUWUulLWUaXtBojbety9lZlcTIVM/pDe\n" \
    "+dHT1Z8wAPf042wwcd09a3j/TvCFBZQH80S2PglpiHvokM8RIzQWolIbA71t9+Da\n" \
    "siZub7B2UQKBgGrF9dnl6tRc9Dpe8aJE+uRdDrZssTiS1g86i1AYCYXuCn6wPaz+\n" \
    "2IYLdGRSdO12/xY2qkZi6trF781sHagHLzcxzqJtPMC7R75wt0H/7CH/ZrqSXK4K\n" \
    "wbU0QmH61ED+8I/1xTvjL4onOZ00AFR6sG6Tygy7qtwXPRq5GN/lYcSNAoGBAM+w\n" \
    "iintB4zPTzdvbwI+6puUYpt46oCx8GqtiVHa586fwtqasRdBOtvLrJReXCVnO4Qx\n" \
    "KTNuYv+sSQ8ctRH4gNoiPO+IokhfpHBJeWKr22aS7k1HkfJlTtMUGWTWmJcbGuNY\n" \
    "bn0m9V2s1vPqnlrQJj/oSyT2KQLn9BrNJ0FYkd6BAoGBAJuXrnZAuHScEFZjt/ZV\n" \
    "46phEbr2cbwdE7VNJLto1wGyr/ukFm4SPNWPyo/jNEfZWaIyYkhcIOxuK1zRInu9\n" \
    "XfIuOqYncmal1RsSphA315HI5GGmWYcoPu8GQSqIPwwkMGa6ZFwdaQmzGxRWCDiA\n" \
    "1jaNr7WdZhWSMDAtyRGZHKdO\n"                                         \
    "-----END PRIVATE KEY-----\n"

#endif // DEFAULT_CERTS_H