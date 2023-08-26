import csv
import os
import numpy as np
from PIL import Image

def CalculateStr(str):
    result = 0
    i = 0
    maxlength = len(str)
    while i < maxlength:
        result += int(str[i],16) * pow(256, i)
        i += 1
    return result

def Transpic(raw, dir, name):
    MAXLENGTH = 16 * 16
    MAX_i = 65  # max row
    MAX_j = 65  # max column

    result = np.zeros((MAX_i,MAX_j,3))
    for i in range(MAX_i):
        for j in range(MAX_j):
            if (i * MAX_i + j < len(raw)):
                result[i][j][0] = int(raw[i * MAX_i + j], 16)
                result[i][j][1] = int(raw[i * MAX_i + j], 16)
                result[i][j][2] = int(raw[i * MAX_i + j], 16)

    im = Image.fromarray(result.astype(np.uint8))
    im.save(dir + '\\' + name)
    return result


def Transcsv(jffs2_raw_inode, csv_file):
    MAXLENGTH = 65 * 65
    with open(csv_file, 'a+', newline='') as f:
        csv_write = csv.writer(f)
        destination = []
        for item in jffs2_raw_inode:
            destination.append(int(item, 16))
        destination = list([ ] + destination + [0] * (MAXLENGTH - len(destination)))
        csv_write.writerow(destination)


def GetRawData(Flash, save_dir, csv_file, work_mode1=0, work_mode2=0, init_number=0):
    index = 0
    dirent_num = 0
    inode_num = 0
    picversion = init_number
    maxlength = len(Flash) - 4
    while index < maxlength:
        if (Flash[index:index + 4] == ['85', '19', '01', 'E0']) or Flash[index:index + 4] == ['85', '19', '1', 'e0']:
            dirent_num += 1

            nodetype = Flash[index:index + 4]
            index += 4
            totlen = Flash[index:index + 4]
            index += 4
            hdr_crc = Flash[index:index + 4]
            index += 4
            pio = Flash[index:index + 4]
            index += 4
            version = Flash[index:index + 4]
            index += 4
            ino = Flash[index:index + 4]
            index += 4
            mctime = Flash[index:index + 4]
            index += 4
            unuseds = Flash[index:index + 4]
            index += 4
            node_crc = Flash[index:index + 4]
            index += 4
            name_crc = Flash[index:index + 4]
            index += 4
            name_length = CalculateStr(totlen) - 40
            name = Flash[index:index + name_length]
            index += name_length
            jffs2_raw_dirent = nodetype + totlen + hdr_crc + pio + version + ino + mctime + unuseds + node_crc + name_crc + name

            print('Found a jffs2_raw_dirent', dirent_num, ', length:', len(jffs2_raw_dirent))
            print(jffs2_raw_dirent)
            picversion += 1
            if work_mode1:
                Transpic(jffs2_raw_dirent, save_dir, str(picversion) + '.jpg')
            if work_mode2:
                Transcsv(jffs2_raw_dirent, csv_file)

        elif Flash[index:index + 4] == ['85', '19', '02', 'E0'] or Flash[index:index + 4] == ['85', '19', '2', 'e0']:
            inode_num += 1
            nodetype = Flash[index:index + 4]
            index += 4
            totlen = Flash[index:index + 4]
            index += 4
            hdr_crc = Flash[index:index + 4]
            index += 4
            ino = Flash[index:index + 4]
            index += 4
            version = Flash[index:index + 4]
            index += 4
            mode = Flash[index:index + 4]
            index += 4
            ugid = Flash[index:index + 4]
            index += 4
            isize = Flash[index:index + 4]
            index += 4
            atime = Flash[index:index + 4]
            index += 4
            mtime = Flash[index:index + 4]
            index += 4
            ctime = Flash[index:index + 4]
            index += 4
            offset = Flash[index:index + 4]
            index += 4
            csize = Flash[index:index + 4]
            index += 4
            dsize = Flash[index:index + 4]
            index += 4
            comprs = Flash[index:index + 4]
            index += 4
            data_crc = Flash[index:index + 4]
            index += 4
            node_crc = Flash[index:index + 4]
            index += 4
            data_length = CalculateStr(totlen) - 68
            data = Flash[index:index + data_length]
            index += data_length
            jffs2_raw_inode = nodetype + totlen + hdr_crc + ino + version + mode + ugid + isize + atime + mtime + ctime + offset + csize + dsize + comprs + data_crc + node_crc + data

            print('Found a jffs2_raw_inode',inode_num, ', length:', len(jffs2_raw_inode))
            print(jffs2_raw_inode)
            picversion += 1
            if work_mode1:
                Transpic(jffs2_raw_inode, save_dir, str(picversion) + '.jpg')
            if work_mode2:
                Transcsv(jffs2_raw_inode, csv_file)
            """
            if data_length != 0:
                picversion += 1
                print('Found a jffs2_raw_inode', inode_num, ', length:', len(jffs2_raw_inode))
                print(jffs2_raw_inode)
                if work_mode1:
                    Transpic(jffs2_raw_inode, save_dir, str(picversion)+'.jpg')
                if work_mode2:
                    Transcsv(jffs2_raw_inode, csv_file)
            """
        else:
            index += 1
    return picversion



def main_0(file, Test_dir, csv_file):
    path = os.getcwd()
    #file = path + "\\test_data" + "\\PandoraBox_without_malware.bin"

    #Test_dir = path + "\\Test"  # output path for image
    #csv_file = Test_dir + "\\test.csv"  # output csv for node

    new_data = []
    init_number = 0

    f = open(file,'rb')
    Data = f.read()
    #print(Data)
    for i in Data:
        new_data.append(str(hex(i))[2:])
    print("OK")
    init_number = GetRawData(new_data, Test_dir, csv_file, work_mode1=1, work_mode2=1)


# Candidate
def main_1():
    path = os.getcwd()
    file_0 = path + "\\test_data" + "\\PandoraBox_without_malware.bin"
    file_1 = path + "\\test_data" + "\\PandoraBox_with_malware.bin"

    Test_dir = path + "\\Test_mal"  # output path for image
    csv_file = Test_dir + "\\test.csv"  # output csv for node

    new_data = []
    init_number = 0

    f_0 = open(file_0, 'rb')
    f_1 = open(file_1, 'rb')
    Data_0 = f_0.read()
    Data_1 = f_1.read()
    #print(Data)
    for i in range(len(Data_0)):
        if Data_0[i] != Data_1[i]:
             #new_data.append(str(hex(Data_1[i]))[2:])
            Data_1 = Data_1[i-1:]
            break
    for i in Data_1:
        new_data.append(str(hex(i))[2:])
    print("OK")
    init_number = GetRawData(new_data, Test_dir, csv_file, work_mode1=1, work_mode2=1)


if __name__ == "__main__":
    path = os.getcwd()
    file_0 = path + "\\flash\\old.bin"
    file_1 = path + "\\flash\\new.bin"

    Test_dir_0 = path + "\\old_nodes"  # output path for image
    csv_file_0 = path + "\\flash\\old_flash.csv"  # output csv for node

    Test_dir_1 = path + "\\new_nodes"  # output path for image
    csv_file_1 = path + "\\flash\\new_flash.csv"  # output csv for node


    #main_0(file_0, Test_dir_0, csv_file_0)
    main_0(file_1, Test_dir_1, csv_file_1)
    #main_1()