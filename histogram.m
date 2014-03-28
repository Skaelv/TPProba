load('LeastSignificant.txt')
load('MostSignificant.txt')
load('VonNeumann.txt')
load('MersenneTwister.txt')
load('AES.txt')

figure(1);
subplot(1,5,1); hist(LeastSignificant,16);
subplot(1,5,2); hist(MostSignificant,16);
subplot(1,5,3); hist(VonNeumann,16);
subplot(1,5,4); hist(MersenneTwister);
subplot(1,5,5); hist(AES);