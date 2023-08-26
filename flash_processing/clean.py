import os
import hashlib
import csv


def Write_Text(file_name,contant):
    # file_name = 'test.txt'
    with open(file_name,"a+") as f:
        f.writelines(contant)
        f.writelines("\n")


def getMD5(file, readLen = 1024*20):
    md5code = hashlib.md5()
    with open(file, 'rb') as f:
        data = f.read()

    md5code.update(data)

    ret = md5code.hexdigest()
    return ret


def main():
    path = os.getcwd()

    Test_dir_0 = path + "\\old_nodes"  # without mal
    Test_dir_1 = path + "\\new_nodes"  # with mal
    csv_file = path + "\\append.csv"

    md5list = []
    notmal = []
    mal = []
    for i in os.listdir(Test_dir_0):
        print(i)
        md5list.append(getMD5(os.path.join(Test_dir_0, i)))
    for i in os.listdir(Test_dir_1):
        if getMD5(os.path.join(Test_dir_1, i)) in md5list:
            print(i)
            notmal.append(i)
        elif i != "test.csv":
            mal.append(i)

    with open('flash\\new_flash.csv', 'r') as f1:
        reader = csv.reader(f1)
        result = list(reader)
        f1.close()
    # sort list
    mal.sort(key=lambda x: int(x[:-4]))
    notmal.sort(key=lambda x: int(x[:-4]))
    with open(csv_file, 'a+', newline='') as f:
        csv_write = csv.writer(f)
        for i in mal:
            Write_Text("1.txt", i)
            print (int(i[:-4]))
            print (result[int(i[:-4])])
            csv_write.writerow(result[int(i[:-4])])
        for i in notmal:
            Write_Text("0.txt", i)
        f.close()


if __name__ == '__main__':
    main()