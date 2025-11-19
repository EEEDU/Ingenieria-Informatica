import random, zipfile

# Parámetros
num_events = 10000
instructions = ["A", "C", "D", "O", "E", "I", "B", "LD", "LT"]
weights = [0.45, 0.15, 0.10, 0.05, 0.15, 0.05, 0.03, 0.01, 0.01]

# Funciones auxiliares
def random_name():
    prefix = random.choice(["motor", "sensor", "valvula", "puerta", "zona", "compresor", "bomba", "alarma", "panel", "sistema"])
    return f"{prefix}_{random.randint(1,999)}"

def random_desc(name):
    verbs = ["Activado", "Desactivado", "Revisado", "Reiniciado", "Ejecutado", "Comprobado", "Abierto", "Cerrado", "Detectado"]
    return f"{random.choice(verbs)} {name} correctamente"

def random_dependencia(existing_names):
    if random.random() < 0.9 and existing_names:
        return random.choice(list(existing_names))
    else:
      return random_name()

lines = []
existing_names = set()

for _ in range(num_events):
    instr = random.choices(instructions, weights)[0]
    lines.append(instr)

    if instr == "A":
        name = random_name()
        desc = random_desc(name)
        priority = random.randint(1, 9)
        dep_type = random.choice(["DEPendiente", "INDependiente"])
        dependencia = random_dependencia(existing_names) if dep_type == "DEPendiente" else "-.-.-.-.-"
        lines += [name, desc, str(priority), dep_type, dependencia]
        existing_names.add(name)

    elif instr == "C":
        if existing_names:
            name = random.choice(list(existing_names))
        else:
            name = random_name()
            existing_names.add(name)
        desc = random_desc(name)
        priority = random.randint(1, 9)
        lines += [name, desc, str(priority)]

    elif instr == "D":
        if existing_names:
            name = random.choice(list(existing_names))
        else:
            name = random_name()
            existing_names.add(name)
        dependencia = random_dependencia(existing_names)
        lines += [name, dependencia]

    elif instr in ["O", "E", "I", "B", "LD"]:
        if existing_names and random.random() < 0.7:
            name = random.choice(list(existing_names))
        else:
            name = random_name()
        lines.append(name)

    elif instr == "LT":
        continue

# Guardar archivo
path_txt = "./entrada.txt"
with open(path_txt, "w") as f:
    f.write("\n".join(lines) + "\n")
