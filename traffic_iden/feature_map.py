
import os
import numpy as np
import random

input_dir='XXX'
output_dir='XXX'

att=np.zeros((num_att,512,512)).astype(np.uint8)
ben=np.zeros((num_ben,512,512)).astype(np.uint8)

rat=ratio

attlie=os.listdir(input_dir+'attack/')
benlie=os.listdir(input_dir+'benign/')

random.shuffle(attlie)
random.shuffle(benlie)

for i1 in range(len(attlie)):
  att[i1]=np.load(input_dir+'attack/'+attlie[i1])


for i1 in range(len(benlie)):
  ben[i1]=np.load(input_dir+'benign/'+benlie[i1])



tesx=np.concatenate((att[:int(att.shape[0]*rat)],ben[:int(ben.shape[0]*rat)]),axis=0)
tesy=np.concatenate((np.array([1]*int(att.shape[0]*rat)),np.array([0]*int(ben.shape[0]*rat))),axis=0)
trax=np.concatenate((att[int(att.shape[0]*rat):],ben[int(ben.shape[0]*rat):]),axis=0)
tray=np.concatenate((np.array([1]*int(att.shape[0]-int(att.shape[0]*rat))),np.array([0]*int(ben.shape[0]-int(ben.shape[0]*rat)))),axis=0)

np.save(output_dir+'tesx.npy',tesx.astype(np.uint8))
np.save(output_dir+'tesy.npy',tesy)
np.save(output_dir+'trax.npy',trax.astype(np.uint8))
np.save(output_dir+'tray.npy',tray)


import json

f=open(output_dir+'attlie.json','w')
f.write(json.dumps(attlie))
f.close()

f=open(output_dir+'benlie.json','w')
f.write(json.dumps(benlie))
f.close()


