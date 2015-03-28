
#from sklearn import svm
from sklearn import preprocessing
#from sklearn.decomposition import PCA
from sklearn import linear_model
from sys import stdin
#from sys import stdout
import numpy

#fin = open('D:\\Quora\input00.txt', 'r')

firststr = stdin.readline()

#firststr = fin.readline()
vec = firststr.split(' ');
Ntrain = int(vec[0]);
Nf = int(vec[1]);
#for line in sys.stdin:

xtrain = numpy.zeros((Ntrain, Nf));
ytrain = numpy.zeros(Ntrain);

for i in range(0, Ntrain):
#    line = fin.readline();
    line = stdin.readline();
    vec = line.replace(':', ' ').split(' ');
    ytrain[i] = float(vec[1]);
    for j in range(1, Nf + 1):
        xtrain[i][j-1] = float(vec[2*j + 1]);


#Ntest = int(fin.readline());
Ntest = int(stdin.readline());
xtest = numpy.zeros((Ntest, Nf));
idtest = list('' for i in range(Ntest))
for i in range(0, Ntest):
#    line = fin.readline();
    line = stdin.readline();
    vec = line.replace(':', ' ').split(' ');
    idtest[i] = vec[0];
    for j in range(1, Nf + 1):
        xtest[i][j-1] = float(vec[2*j]);
    
#fin.close();    
xtotal = numpy.concatenate((xtrain, xtest));
xtotal = preprocessing.scale(xtotal)


#pca = PCA(n_components=15)
#xtotal = pca.fit_transform(xtotal)
clf = linear_model.LogisticRegression(C=100, penalty='l2', tol=0.001)
clf.fit(xtrain, ytrain)
  
#ytest = clf.predict(xtest)
nratio = (Ntrain - sum(ytrain))/2/Ntrain;
ypos = clf.predict_proba(xtest)
threth = numpy.percentile(ypos[:,1], 100*(1 - nratio));

for i in range(0, Ntest):
    if ypos[i,1]  > threth:
        print(idtest[i] + ' +1')
    else:
        print(idtest[i] + ' -1')
        
