# -*- coding: utf-8 -*-
"""
Created on Fri Mar 06 19:36:06 2015

@author: Bao
"""

from sklearn import svm
from sklearn import preprocessing
from sklearn.decomposition import PCA
from sklearn import linear_model
from sys import stdin
import numpy
from sklearn.ensemble import AdaBoostClassifier
from sklearn.ensemble import RandomForestClassifier
from sklearn.ensemble import ExtraTreesClassifier
from sklearn.tree import DecisionTreeClassifier
from sklearn.ensemble import BaggingClassifier
from sklearn.neighbors import KNeighborsClassifier
from sklearn.preprocessing import PolynomialFeatures
from sklearn.linear_model import Perceptron

#fin = open('D:\\Quora\input01.txt', 'r')

firststr = stdin.readline()

#firststr = fin.readline()
vec = firststr.split(' ');
Ntrain = int(vec[0]);
Nf = int(vec[1]);
#for line in sys.stdin:

xtrain = numpy.zeros((Ntrain, Nf));
ytrain = numpy.zeros(Ntrain);

for i in range(0, Ntrain):
    line = stdin.readline();
    vec = line.replace(':', ' ').split(' ');
    ytrain[i] = float(vec[1]);
    for j in range(1, Nf + 1):
        xtrain[i][j-1] = float(vec[2*j + 1]);


Ntest = int(stdin.readline());
xtest = numpy.zeros((Ntest, Nf));
idtest = list('' for i in range(Ntest))
for i in range(0, Ntest):
    line = stdin.readline();
    vec = line.replace(':', ' ').split(' ');
    idtest[i] = vec[0];
    for j in range(1, Nf + 1):
        xtest[i][j-1] = float(vec[2*j]);
    
 
xtotal = numpy.concatenate((xtrain, xtest));
#xtotal = preprocessing.scale(xtotal)
for i in range(0,Nf):
    m = numpy.mean(xtotal[:,i]);
    xtotal[:,i] = xtotal[:,i] - m;
    s = numpy.std(xtotal[:,i]);
    if(s > 0):
        xtotal[:,i] = xtotal[:,i]/s;

#pca = PCA(n_components=10)
#xtotal = pca.fit_transform(xtotal)

xtrain = xtotal[0:Ntrain,:];
xtest = xtotal[Ntrain:Ntrain + Ntest,:];


#clf = svm.SVC(class_weight = dict([(-1,100),(1,1)]))
#clf = svm.SVC(kernel = 'linear');
#clf = linear_model.LogisticRegression(C=100, penalty='l1', tol=0.0001)
clf =  AdaBoostClassifier(n_estimators = 200)
#clf = ExtraTreesClassifier(n_estimators=100, max_depth=None, min_samples_split=1, random_state=0)
#clf = BaggingClassifier(KNeighborsClassifier(),  max_samples=0.5, max_features=0.5)

#clf = Perceptron(fit_intercept=False, n_iter=10)
clf.fit(xtrain, ytrain)
  


ytest = clf.predict(xtest)

for i in range(0, Ntest):
    if ytest[i]  > 0:
        print(idtest[i] + ' +1')
    else:
        print(idtest[i] + ' -1')

