function pathVis(env_file,varargin)
  % Reads the environment env1.config sol.txt file output by the planner and visualizes it
  % Assumes environment is created from a binary image. Direction arrows
  % visualized are just for that (visualization) -- they do not relate to
  % anything physical.

  fid = fopen(env_file);
  % Get environment size
  cells = fgets(fid);
  cells(1:23) = [];
  cells = str2num(cells);
  for i = 1:3
    temp = fgets(fid)
  end
  % Get resolution
  resolution = fgets(fid)
  resolution(1:17)=[];
  resolution = str2num(resolution);
  for i = 1:5
    temp = fgets(fid);
  end

  env = fscanf(fid,'%d');
  size(env)
  env = reshape(env,cells(1),cells(2));
  fclose(fid);
  % Draw the environment
  if(nargin <= 3)
    using_bitmap = 0;
    imshow(uint8(255*env));axis xy;
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

  % Read the solution file
  if(nargin > 1)
    sol_file = varargin{1};
    fid = fopen(sol_file);
    resolution
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
