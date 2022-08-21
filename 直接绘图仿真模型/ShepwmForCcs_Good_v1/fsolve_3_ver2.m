clc;
clear;


m = 0.9;

% [x,fval,exitflag] = fsolve(@(x) myfun_5(x,m),[0.7854;0.9599;1.1345;1.3090;1.4835])
[x,fval,exitflag] = fsolve(@(x) myfun_3_ver2(x,m),[0.58;1.1;1.24])


y = x*180/pi;