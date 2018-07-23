import math

(a, b, tx) = map(lambda x: float(x), input().split())
(c, d, ty) = map(lambda x: float(x), input().split())

xc = tx
yc = ty
sign = lambda x: 1 if x == 0 else x / abs(x)
sx = sign(a) * math.sqrt(a * a + b * b)
sy = sign(d) * math.sqrt(c * c + d * d)
psi = math.atan2(c, d)

print("xc: {}\nyc: {}\nsx: {}\nsy: {}\npsi: {}".format(xc, yc, sx, sy, psi))