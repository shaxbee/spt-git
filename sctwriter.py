from struct import Struct

class BinaryWriter(object):
    uIntFormat = Struct("I")
    vec3fFormat = Struct("fff")
    vec3dFormat = Struct("ddd")

    def __init__(self, ifile):
        self.__input = ifile

    def writeArray(self, fmt, data, count):
        buf = bytearray(fmt.size * count)
        offset = 0
        for values in data:
            fmt.pack_into(buf, offset, *values)
            offset += fmt.size
        self.__input.write(name)

    def writeVarArray(self, data):
        for fmt, values in data:
            self.__input.write(fmt.pack(*values))

    def writeChunk(self, name, data):
        self.__input.write(name)
        self.writeUInt(len(data))
        self.writeString(data)

    def writeString(self, value):
        self.writeUInt(len(value))
        self.__input.write(value)

    def writeUInt(self, value):
        self.writeFmt(uIntFormat, value)

    def writeVec3f(self, value):
        self.writeFmt(vec3fFormat, value.as_tuple())

    def writeVec3d(self, value):
        self.writeFmt(vec3dFormat, value.as_tuple())

    def writeFmt(self, fmt, values):
        self.__input.writer(fmt.pack(*values))

__trackFormats = [
    Struct("B 3f3f"), # straight
    Struct("B 3f3f3f3f") # bezier
]

__switchFormats = [
    Struct("B 3f3f 3f3f"), # straight, straight
    Struct("B 3f3f 3f3f3f3f"), # straight, bezier
    Struct("B 3f3f3f3f 3f3f"), # bezier, straight
    Struct("B 3f3f3f3f 3f3f3f3f") # bezier, bezier
]

def __tracksGen(tracks, offset):
    for track in tracks:
        yield track.path.points(offset)

def __switchGen(switches, offset):
    for switch in switches:
        fmt = __switchFormats[switch.straight.kind + switch.diverted.kind * 2]
        yield (fmt, switch.straight.points(offset) + switch.diverted.points(offset))

class SectorWriter(BinaryWriter):
    def __init__(self, ifile):
        self.__init__(ifile)

    def __writeTrackList(self):
        for kind in range(2):
            size = len(self.data.tracks[PathKind.STRAIGHT])
            self.writeUInt(size)
            self.writeArray(__trackFormats[kind], __tracksGen(self.data.tracks[kind], self.__offset), size)

    def __writeSwitchList(self):
        self.writeUInt(len(self.data.switches))
        for switch in self.data.switches:
            self.__writePath(switch.straight)
            self.__writePath(switch.diverted)
            self.__writer.writeString(switch.position)
