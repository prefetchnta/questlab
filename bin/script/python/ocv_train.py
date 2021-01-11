#-*- coding: utf-8 -*-
##################################
#   OpenCV 训练辅助工具
#
#   创建时间: 2018-03-15
##################################

from PIL import Image
import hashlib, os, sys

# 参数判断
if len(sys.argv) != 6:
    print("Usage: ocv_train.py <rootDir> <width> <height> <minHitRate> <maxFalseAlarmRate>")
    exit()

# 拼装目录
ROOT_DIR = sys.argv[1]
INPUT_NEG_DIR = ROOT_DIR + "\\raw-neg\\"
INPUT_POS_DIR = ROOT_DIR + "\\raw-pos\\"
OUTPUT_NEG_DIR = ROOT_DIR + "\\neg\\"
OUTPUT_POS_DIR = ROOT_DIR + "\\pos\\"
OUTPUT_NEG_FILE = ROOT_DIR + "\\neg.txt"
OUTPUT_POS_FILE = ROOT_DIR + "\\pos.txt"

# 拼装参数
arg_w_h = "-w %u -h %u" % (int(sys.argv[2]), int(sys.argv[3]))
arg_set = "-minHitRate %s -maxFalseAlarmRate %s" % (sys.argv[4], sys.argv[5])

#######################################
#   文件哈希 (用于统一文件名)
#######################################
def file_hash(name):
    fp = open(name, "rb")
    dat = fp.read()
    fp.close()
    return hashlib.sha1(dat).hexdigest()

# 移动负样本
for neg_file in os.listdir(INPUT_NEG_DIR):
    name, fext = os.path.splitext(neg_file)
    full_path = INPUT_NEG_DIR + neg_file
    move_path = OUTPUT_NEG_DIR + file_hash(full_path) + fext
    if os.path.exists(move_path):
        os.remove(move_path)
    os.rename(full_path, move_path)

# 生成负样本列表
total_neg = 0
f = open(OUTPUT_NEG_FILE, "w")
for neg_file in os.listdir(OUTPUT_NEG_DIR):
    try:
        full_path = OUTPUT_NEG_DIR + neg_file
        img = Image.open(full_path)
        pic_w, pic_h = img.size
        f.write("neg\\" + neg_file + "\n")
        total_neg += 1
    except:
        print("invalid neg image file: " + neg_file)
        os.remove(full_path)
f.close()

# 移动正样本
for pos_file in os.listdir(INPUT_POS_DIR):
    name, fext = os.path.splitext(pos_file)
    full_path = INPUT_POS_DIR + pos_file
    move_path = OUTPUT_POS_DIR + file_hash(full_path) + fext
    if os.path.exists(move_path):
        os.remove(move_path)
    os.rename(full_path, move_path)

# 生成正样本列表
total_pos = 0
f = open(OUTPUT_POS_FILE, "w")
for pos_file in os.listdir(OUTPUT_POS_DIR):
    try:
        full_path = OUTPUT_POS_DIR + pos_file
        img = Image.open(full_path)
        pic_w, pic_h = img.size
        f.write("pos\\" + pos_file + " 1 0 0 " + str(pic_w) + " " + str(pic_h) + "\n")
        total_pos += 1
    except:
        print("invalid pos image file: " + pos_file)
        os.remove(full_path)
f.close()

# 输出创建样本命令
exec_str = "@opencv_createsamples -vec img.vec -info pos.txt %s -num %u\n" % (arg_w_h, total_pos)
f = open(ROOT_DIR + "\\vector.bat", "w")
f.write(exec_str)
f.write("@pause\n")
f.close()

# 根据建议设置正负样本数量
total_pos = (total_pos * 9) / 10
total_neg = (total_pos * 3)

# 输出几种训练命令
exec_str = "@opencv_traincascade -data .\\out\\ -vec img.vec -bg neg.txt %s %s -numPos %u -numNeg %u -numThreads 5 -mode ALL\n" % (arg_w_h, arg_set, total_pos, total_neg)
f = open(ROOT_DIR + "\\train.bat", "w")
f.write(exec_str)
f.write("@pause\n")
f.close()

exec_str = "@opencv_traincascade -data .\\lbp\\ -vec img.vec -bg neg.txt %s %s -numPos %u -numNeg %u -numThreads 5 -featureType LBP\n" % (arg_w_h, arg_set, total_pos, total_neg)
f = open(ROOT_DIR + "\\train_lbp.bat", "w")
f.write(exec_str)
f.write("@pause\n")
f.close()

exec_str = "@opencv_traincascade -data .\\hog\\ -vec img.vec -bg neg.txt %s %s -numPos %u -numNeg %u -numThreads 5 -featureType HOG\n" % (arg_w_h, arg_set, total_pos, total_neg)
f = open(ROOT_DIR + "\\train_hog.bat", "w")
f.write(exec_str)
f.write("@pause\n")
f.close()
