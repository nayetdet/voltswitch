from pathlib import Path

Import("env")

env_file = Path(env["PROJECT_DIR"]) / ".env"
if not env_file.is_file():
    raise FileNotFoundError("Environment file not found")

with env_file.open() as f:
    for line in f:
        line = line.strip()
        if not line or line.startswith("#"):
            continue

        key, value = [segment.strip().strip('"').strip("'") for segment in line.split("=", 1)]
        env.Append(CPPDEFINES=[(key, f'\\"{value}\\"')])
