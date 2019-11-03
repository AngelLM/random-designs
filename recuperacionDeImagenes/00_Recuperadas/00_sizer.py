import os

files = os.listdir(".")
for file in files:
    if not "00_Mas1MB" in file and not "00_Menos1MB" in file and not "00_sizer.py" in file:
        if os.path.getsize(file)>1000000:
            os.rename(file, "00_Mas1MB/" + file)
        else:
            os.rename(file, "00_Menos1MB/" + file)

print("------------------")
print("------------------")
print("------------------")
print("-------DONE-------")
print("------------------")
print("------------------")
print("------------------")
