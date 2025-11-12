import socket

s = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
s.connect( ('127.0.0.1', 2112) )

while True:
    inp = input( '> ' )
    if inp == 'exit': break
    s.sendall( inp.encode() )
    outp = s.recv(1024).decode()
    print( f'< {outp}' )
