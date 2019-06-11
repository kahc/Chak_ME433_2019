function data = read_plot_matrix(mySerial)
  nsamples = 100;       % hardcode this
  data = zeros(nsamples,5);               % 5 values: index, raw, MAF, IIR, and FIR
  for i=1:nsamples
    data(i,:) = fscanf(mySerial,'%d %d %d %d %d'); % read in data from PIC32; assume ints, in mA
    times(i) = data(i, 1);                 % use sample number instead of time
  end
  if nsamples > 1						        
    plot(times,data(:,2:5));            % plot the raw and MAF
  else
    fprintf('Only 1 sample received\n')
    disp(data);
  end
  % compute the average error
  %score = mean(abs(data(:,1)-data(:,2)));
  %fprintf('\nAverage error: %5.1f deg\n',score);
  %title(sprintf('Average error: %5.1f deg',score));
  %ylabel('z-accel');
  %xlabel('time (ms)');  
  legend('raw', 'MAF', 'IIR', 'FIR');
end
