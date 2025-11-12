import socket

s = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
s.connect( ('127.0.0.1', 2112) )

while True:
    inp = input( '> ' )
    if inp == 'exit': break

    # in order to work, I had to encode the input because only accepted bytes-like object
    s.sendall( inp.encode())
    outp = s.recv(1024)

    print( f'< {outp}' )

s.close()
