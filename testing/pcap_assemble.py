import os
import re

arr = os.listdir()


myregex = re.compile(r"\/\/file(\d*)")
li = ["" for i in range(1000)]
for f in arr:
	with open(f, "r") as myfile:
		try:
			content = myfile.read()
			result = re.findall(myregex, content)
			print(f, result)
			li[int(result[0])] = content;
		except Exception as e:
			print(f, "Exception : ", e)
outfile = open("../outfile.c", "w")
print (li)
outfile.write("\n".join(li))
