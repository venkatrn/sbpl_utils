function pathVis(env_file,varargin)
  % Reads the environment env1.config sol.txt file output by the planner and visualizes it

  fid = fopen(env_file);
  % Get environment size
  cells = fgets(fid);
  cells(1:23) = [];
  cells = str2num(cells);
  for i = 1:4
    temp = fgets(fid);
  end
  % Get resolution
  resolution = fgets(fid);
  resolution(1:17)=[];
  resolution = str2num(resolution);
  for i = 1:5
    temp = fgets(fid);
  end

  env = fscanf(fid,'%d');
  env = reshape(env,cells(1),cells(2));
  fclose(fid);
  % Draw the environment
  if(nargin <= 3)
    using_bitmap = 0;
    imshow(uint8(env));axis xy;
  else
    using_bitmap = 1;
    im = imread(varargin{3});
    scalex = size(im,1)/cells(1);
    scaley = size(im,2)/cells(2);
    for i=1:3
      im(:,:,i) = flipud(im(:,:,i));
    end
    imshow(im);axis xy;

  end



  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  % Read the beams file
  if(nargin > 1)
    beams_file = varargin{1};
    fid = fopen(beams_file);
    temp = fgets(fid);
    temp(1:11) = [];
    num_beams = str2num(temp);
    beam_ids = cell(1,num_beams);
    beams = zeros(num_beams,3); 
    for i=1:num_beams
      temp = fgets(fid);   
      beam_ids{i} = temp(1);
      temp(1) = [];
      beams(i,:) = str2num(temp);
    end

    if using_bitmap == 1
      beams(:,1) = beams(:,1)*scalex;
      beams(:,2:3) = beams(:,2:3)*scalex;
    end

    for i = 1:num_beams
      hold on;
      plot([beams(i,1) beams(i,1)],[beams(i,2) beams(i,3)],'r--','LineWidth',2);
      text_handle = text(beams(i,1),(beams(i,2)+beams(i,3))/2,beam_ids{i});
      set(text_handle,'FontSize',16,'FontWeight','bold','Color','k','BackgroundColor',[1.0 0 0],'EdgeColor','w');
    end

  end
  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  % Read the solution file
  if(nargin > 2)
    sol_file = varargin{2};
    fid = fopen(sol_file);
    path = dlmread(sol_file)/resolution;
    if(using_bitmap == 1)
      path(:,2) = path(:,2)*scalex;
      path(:,1) = path(:,1)*scaley;
    end

    % Draw the path
    hold on;
    plot(path(:,2),path(:,1),'b-','LineWidth',3);axis xy;
    %plot(path(1:10:end,2),path(1:10:end,1),'g<','MarkerSize',6);axis xy;
    path = path(1:10:end,:);
    vec = [path(2:end,2)'-path(1:end-1,2)';
    path(2:end,1)'-path(1:end-1,1)'];
    hold on;
    size(path)
    size(vec)
    quiver(path(1:end-1,2),path(1:end-1,1),vec(1,:)',vec(2,:)',1,'r-');


    % Draw starting point
    hold on;
    plot(path(1,2),path(1,1),'cs','MarkerSize',10,'MarkerFace','c');axis xy;
  end
