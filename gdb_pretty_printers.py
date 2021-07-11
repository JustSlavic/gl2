import re


class Math_Vector2_PrettyPrinter:
    "Prints math::vector2"

    def __init__(self, v):
        self.value = v

    def to_string(self):
        return "math::vector2({}, {})".format(self.value['x'], self.value['y'])

    def display_hint(self):
        return "math::vector2"


class Math_Vector3_PrettyPrinter:
    "Prints math::vector3"

    def __init__(self, v):
        self.value = v

    def to_string(self):
        return "math::vector3({}, {}, {})".format(self.value['x'], self.value['y'], self.value['z'])

    def display_hint(self):
        return "math::vector3"


class Math_Vector4_PrettyPrinter:
    "Prints math::vector4"

    def __init__(self, v):
        self.value = v

    def to_string(self):
        return "math::vector4({}, {}, {}, {})".format(self.value['x'], self.value['y'], self.value['z'], self.value['w'])

    def display_hint(self):
        return "math::vector4"

def Math_Vector_Lookup(v):
    lookup_tag = v.type.tag
    if lookup_tag == None:
        return None

    regex = re.compile(r"^math::vector(\d)$")
    match = regex.match(lookup_tag)
    
    if match:
        n = int(match.group(1))
        if n == 2:
            return Math_Vector2_PrettyPrinter(v)
        if n == 3:
            return Math_Vector3_PrettyPrinter(v)
        if n == 4:
            return Math_Vector4_PrettyPrinter(v)

    return None


gdb.pretty_printers.append(Math_Vector_Lookup)
