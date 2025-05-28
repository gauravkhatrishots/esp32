import ssl
import socket
from pprint import pprint

hostname = 'api.telegram.org'
port = 443

context = ssl.create_default_context()

with socket.create_connection((hostname, port)) as sock:
    with context.wrap_socket(sock, server_hostname=hostname) as ssock:
        cert = ssock.getpeercert()
        print("Server certificate:")
        pprint(cert)
