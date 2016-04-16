function plotMethod(num,mat, ycolumn,xcolumn,type,name,xname,yname)

[r,c]=size(mat);
n=ones(8);
for npcount=1:8
    
for i=1:r
        if (mat(i,4)==npcount && mat(i,1)==type) 
         y(npcount,n(npcount))=mat(i,ycolumn);
         x(npcount,n(npcount))=mat(i,xcolumn);
         n(npcount)=n(npcount)+1;
        end
end
end
figure(num);

loglog(x(2,:),y(2,:),'-*',x(3,:),y(3,:),'-*',x(4,:),y(4,:),'-*',x(5,:),y(5,:),'-*',x(6,:),y(6,:),'-*',x(7,:),y(7,:),'-*',x(8,:),y(8,:),'-*');
legend('2','3','4','5','6','7','8');
title(name);
xlabel(xname);
ylabel(yname);
%axis equal;
%grid on;

end