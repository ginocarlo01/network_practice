import socket

s = socket.socket( socket.AF_INET, socket.SOCK_STREAM )
s.bind( ('127.0.0.1', 2112) )
s.listen( 5 )

while True:
    conn, addr = s.accept()
    print( f'new connection {conn} addr {addr}' )
    while True:
        data = conn.recv( 1024 )
        if not data:
            print( f'no more data, dropping connection' )
            break
        print( f'> {data}' )
        conn.sendall( data )
