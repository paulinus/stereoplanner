class OBJ:
    def __init__(self, filename, swapyz=False):
        """Loads a Wavefront OBJ file. """
        self.vertices = []
        self.normals = []
        self.texcoords = []
        self.faces = []
 
        material = None
        for line in open(filename, "r"):
            if line.startswith('#'): continue
            values = line.split()
            if not values: continue
            if values[0] == 'v':
                v = map(float, values[1:4])
                if swapyz:
                    v = v[0], v[2], v[1]
                self.vertices.append(v)
            elif values[0] == 'vn':
                v = map(float, values[1:4])
                if swapyz:
                    v = v[0], v[2], v[1]
                self.normals.append(v)
            elif values[0] == 'vt':
                self.texcoords.append(map(float, values[1:3]))
            elif values[0] in ('usemtl', 'usemat'):
                material = values[1]
            elif values[0] == 'mtllib':
                self.mtl = MTL(values[1])
            elif values[0] == 'f':
                face = []
                texcoords = []
                norms = []
                for v in values[1:]:
                    w = v.split('/')
                    face.append(int(w[0]))
                    if len(w) >= 2 and len(w[1]) > 0:
                        texcoords.append(int(w[1]))
                    else:
                        texcoords.append(0)
                    if len(w) >= 3 and len(w[2]) > 0:
                        norms.append(int(w[2]))
                    else:
                        norms.append(0)
                self.faces.append((face, norms, texcoords, material))
 



import sys

if len(sys.argv) < 3:
  print '''
    USAGE: %s input.obj output.geo
    ''' % sys.argv[0]
  sys.exit(1)

o = OBJ(sys.argv[1])

vnindex = {}
vertices = []
normals = []
faces = []
for f in o.faces:
  newf = []
  for i in range(3):
    vn = (f[0][i] - 1, f[1][i] - 1)
    if not vn in vnindex:
      vnindex[vn] = len(vertices)
      vertices.append(o.vertices[vn[0]] + [1.0])
      normals.append(o.normals[vn[1]])
    newf.append(vnindex[vn])
  faces.append(newf)



import struct, operator

fout = open(sys.argv[2], 'w')
fout.write('%d %d\n' % (len(vertices), len(faces)))

vflat = reduce(operator.add, vertices)
nflat = reduce(operator.add, normals)
fflat = reduce(operator.add, faces)

fout.write(''.join([struct.pack('f', v) for v in vflat]))
fout.write(''.join([struct.pack('f', v) for v in nflat]))
fout.write(''.join([struct.pack('H', v) for v in fflat]))

print len(vertices), ' vertices'
print len(normals), ' normals'
print len(faces), ' faces'

print normals
