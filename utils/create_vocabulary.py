#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri May  8 18:19:22 2020

@author: vik748
"""
import os, sys
#if os.path.dirname(os.path.realpath(__file__)) == os.getcwd():
#    sys.path.insert(1, os.path.join(sys.path[0], '..'))
import cv2
#import data
import pyfbow as bow
import time
import glob
import argparse

k = 10
L = 6
nthreads = 1
maxIters = 0
verbose = False

def get_detector_config_string(detector):
    '''
    Get unique config string for detector config
    '''
    get_func_names = sorted([att for att in detector.__dir__() if att.startswith('get')])
    get_vals = [getattr(detector, func_name)() for func_name in get_func_names]
    get_vals_str = [str(val) if type(val)!=float else "{:.2f}".format(val) for val in get_vals]
    return '_'.join(get_vals_str)

parser = argparse.ArgumentParser(description='This script creates a vocabulary from a folder')
parser.add_argument('folder', help='location of folder with images') #go_pro_icebergs_config.yaml
parser.add_argument('-o', '--output', help='name of vocabulary file') #go_pro_icebergs_config.yaml

args = parser.parse_args()
 
image_folder = args.folder

print("Image folder :", image_folder)

voc = bow.Vocabulary(k, L, nthreads, maxIters, verbose)

detector = cv2.ORB_create(nfeatures=2000)

image_names = glob.glob(os.path.join(image_folder,'*'))

des_list = []
for image_name in image_names:

    img = cv2.imread(image_name, cv2.IMREAD_COLOR)

    if img is None:
        print ("Couldn't read image: ",image_name)
        pass

    gr = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    kp, des = detector.detectAndCompute(gr, mask=None)
    print("Detected {} features".format(len(des)))

    des_list.append(des)

st = time.time()
voc.create(des_list[0])


if args.output:
    voc_file_name = args.output
else:
    image_folder_name = os.path.split(os.path.dirname(image_names[0]))[-1].replace(' ','_')
    detector_config_string = get_detector_config_string(detector)
    voc_file_name = image_folder_name + '_' + str(len(image_names)) + '-'+detector_config_string+'.bin'

if os.path.exists(voc_file_name):
    print("Vocaulary file {} exists - Overwriting !!!".format(voc_file_name))

voc.saveToFile(voc_file_name)

print("Created Vocabulary {} in {:.2f} secs".format(voc_file_name,time.time()-st))