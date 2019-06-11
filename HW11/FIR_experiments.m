MAF = zeros(1,6);
for ii = 1:length(MAF)
    MAF(ii) = 1/length(MAF);
end
freqz(MAF);
figure;

%IIR = fir1(1, .1);
%freqz(IIR);