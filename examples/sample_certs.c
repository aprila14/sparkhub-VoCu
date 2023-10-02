/**
 * Copyright (c) 2022 WIZnet Co.,Ltd
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "azure_samples.h"

const char pico_az_connectionString[] = "";

const char pico_az_x509connectionString[] = "HostName=pressure-sensor-paid.azure-devices.net;DeviceId=sparkhub-pressure-sensor-2;x509=true";

const char pico_az_x509certificate[] =
"-----BEGIN CERTIFICATE-----""\n"
"MIIDaDCCAlCgAwIBAgIQFiSFPcSv85PIRj35A4kFLjANBgkqhkiG9w0BAQsFADAT\n"
"MREwDwYDVQQDDAhzcGFya2h1YjAeFw0yMzA5MjkwNjUyMDZaFw0zMzA5MjYwNjUy\n"
"MDZaMFYxCzAJBgNVBAYTAkNIMQ8wDQYDVQQIDAZadXJpY2gxETAPBgNVBAoMCHNw\n"
"YXJraHViMSMwIQYDVQQDDBpzcGFya2h1Yi1wcmVzc3VyZS1zZW5zb3ItMjCCASIw\n"
"DQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMOx0etSieNB5fazYfTIaVd5pw4L\n"
"c5gnVVUxSENXKYMhJVykhR+P18C8YTeDQbVH6r0HdVytj6tAl8LfN54qeRtw46ef\n"
"PAi+8Iw9fsOs4gbtwZ0zIaC/59cDEPz7fRHE4u6ZvBLjlbUrx5l+eXtj5k0c0Szi\n"
"LIhizBQuwRwXlbXDTOnZpppYMzMC4/uTZPVw2DW/lmqoCrQerQG4AxkbBCKSCjdy\n"
"DpvyfBF52P2wZCkJ4+OvY4kqavVncA508V2b+GGD2T9jmw2BU6YaTc+rd7iBwemF\n"
"Dy6QVePQ7XPgExz7INSi/RhTX3XB9MCO+oD1QG6bz32RCmEdEk6dCSSEjgUCAwEA\n"
"AaN1MHMwHwYDVR0jBBgwFoAUJbQ7Umg1bHofPHzZFuT1hUicxZQwDAYDVR0TAQH/\n"
"BAIwADATBgNVHSUEDDAKBggrBgEFBQcDAjAOBgNVHQ8BAf8EBAMCB4AwHQYDVR0O\n"
"BBYEFJgGnFhY5IHpdnp7ol2e2yIvpWV+MA0GCSqGSIb3DQEBCwUAA4IBAQAvnv1Q\n"
"TsSvpnefCFQAef84AcDyLuGkBww99AXurm33iTAXIuAME3FEwaHKrF15XxQbBLdN\n"
"RXX19DuF2vngg7fj4MtC1Qc3yTanJUraWrJE5YbYVHXUkLrhI7bZ2O11hFl/V3Pn\n"
"78IWxhSejEHetC/x+ZPtIAme2vzImGqXLAerQN506AMbj0rOkwmezg18R9dm9akL\n"
"n8OiVkrStpJIU6AaWwkYVEoQ57m3CMI7pS1D/ISrWwnOuyNOyTJsCTToBpCsUZPu\n"
"afSqPAgAdIs5M9rJeTPZ/nuHHRkiiZ86DHIS9Z5xHBxGGfg6hlUMT9cBo0PMb9UE\n"
"KZC3lUePBMEgnB5y\n"
"-----END CERTIFICATE-----";

const char pico_az_x509privatekey[] =
"-----BEGIN PRIVATE KEY-----""\n"
"MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDDsdHrUonjQeX2\n"
"s2H0yGlXeacOC3OYJ1VVMUhDVymDISVcpIUfj9fAvGE3g0G1R+q9B3VcrY+rQJfC\n"
"3zeeKnkbcOOnnzwIvvCMPX7DrOIG7cGdMyGgv+fXAxD8+30RxOLumbwS45W1K8eZ\n"
"fnl7Y+ZNHNEs4iyIYswULsEcF5W1w0zp2aaaWDMzAuP7k2T1cNg1v5ZqqAq0Hq0B\n"
"uAMZGwQikgo3cg6b8nwRedj9sGQpCePjr2OJKmr1Z3AOdPFdm/hhg9k/Y5sNgVOm\n"
"Gk3Pq3e4gcHphQ8ukFXj0O1z4BMc+yDUov0YU191wfTAjvqA9UBum899kQphHRJO\n"
"nQkkhI4FAgMBAAECggEAInn/e6/s7wa7Fv1/cnqkZG7gSY8m7FVxqTKngoWmoJ2E\n"
"UZACjA7xedk2X2tWdjOXXXok6REC0K7ujbx5tfnRoE4BLZTWJMxO4MYswRiCMapN\n"
"g/5wHDvzAmRd59P+stS3DkYzZjiGNYbjAV3Q1TZCaq97AqdOJTEStzONff+BtqXE\n"
"mlxlW1etOtFwmELdhHQJl6YuXXuN1KeSo3v2JBIIbcxAoXWYjjN/8KbR77ysL9rE\n"
"WaspIB/M3bSXyBQKs2XTXPIsGCva6FX6izB4uNnIEmrMtBPT4V3ObBD7SKShFjxg\n"
"PKmTk7g0VX0wE8AGQUeDlavoW0DQIIzRPr3j4OV+gQKBgQDPFbHcOXY1zZBHipKD\n"
"dozZAYvLsN+u0PNLqnilD4FQbk88Ml4txy0urOriUO+kko3Q/ymrFRAkXuavDM3T\n"
"Sv0pcNfLyQ2B+zsRXCy4B4/UVBlRmBEns5JUbS6SRWXT029KWHs6c65PjbjdYeaf\n"
"xSBZ+Vpgul22i0VP90y6v/kLdQKBgQDx616mmS1c0HrqoI11FrxOw8ACKeMRyIge\n"
"tk78H9t/eGGGob3C291VdcqjVuOEHEAdnUWUulLWUaXtBojbety9lZlcTIVM/pDe\n"
"+dHT1Z8wAPf042wwcd09a3j/TvCFBZQH80S2PglpiHvokM8RIzQWolIbA71t9+Da\n"
"siZub7B2UQKBgGrF9dnl6tRc9Dpe8aJE+uRdDrZssTiS1g86i1AYCYXuCn6wPaz+\n"
"2IYLdGRSdO12/xY2qkZi6trF781sHagHLzcxzqJtPMC7R75wt0H/7CH/ZrqSXK4K\n"
"wbU0QmH61ED+8I/1xTvjL4onOZ00AFR6sG6Tygy7qtwXPRq5GN/lYcSNAoGBAM+w\n"
"iintB4zPTzdvbwI+6puUYpt46oCx8GqtiVHa586fwtqasRdBOtvLrJReXCVnO4Qx\n"
"KTNuYv+sSQ8ctRH4gNoiPO+IokhfpHBJeWKr22aS7k1HkfJlTtMUGWTWmJcbGuNY\n"
"bn0m9V2s1vPqnlrQJj/oSyT2KQLn9BrNJ0FYkd6BAoGBAJuXrnZAuHScEFZjt/ZV\n"
"46phEbr2cbwdE7VNJLto1wGyr/ukFm4SPNWPyo/jNEfZWaIyYkhcIOxuK1zRInu9\n"
"XfIuOqYncmal1RsSphA315HI5GGmWYcoPu8GQSqIPwwkMGa6ZFwdaQmzGxRWCDiA\n"
"1jaNr7WdZhWSMDAtyRGZHKdO\n"
"-----END PRIVATE KEY-----";

const char pico_az_id_scope[] = "[ID Scope]";

const char pico_az_COMMON_NAME[] = "[custom-hsm-device]";

const char pico_az_CERTIFICATE[] = 
"-----BEGIN CERTIFICATE-----""\n"
"-----END CERTIFICATE-----";

const char pico_az_PRIVATE_KEY[] = 
"-----BEGIN PRIVATE KEY-----""\n"
"-----END PRIVATE KEY-----";

const char pico_az_securityTypeEnvironmentVariable[] = "[IOTHUB_DEVICE_SECURITY_TYPE]"; // Input either 'connectionString' or 'DPS'

const char pico_az_connectionStringEnvironmentVariable[] = "[IOTHUB_DEVICE_CONNECTION_STRING]";

const char pico_az_dpsIdScopeEnvironmentVariable[] = "[IOTHUB_DEVICE_DPS_ID_SCOPE]";
