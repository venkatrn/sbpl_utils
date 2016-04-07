function generateEnv(image_path)
  env = imread(image_path);
  env = parseBitmap(env);
  env_x = size(env,2);
  env_y = size(env,1);

  % Show the environment
  fig_handle = figure;
  im_handle = imshow(env');axis xy;

  % Default parameters
  obsthresh = 1;
  cost_inscribed_thresh = obsthresh;
  cost_possibly_circumscribed_thresh = 0;
  res = 0.1;
  nominal_vel = 0.1;
  time_to_turn_45_degs = 2.0;

  % Get start/goal from user
  msg_handle = msgbox('Click to select start state');
  [y,x] = ginput(1);
  start_pt = [x y]*res;
  delete(msg_handle);
  r1_handle = rectangle('Position',[y,x,10,10],'FaceColor',[0 .5 0],'EdgeColor','k',...
      'LineWidth',3);
  msg_handle = msgbox('Click to select goal state');
  [y,x] = ginput(1);
  goal_pt = [x y]*res;
  r2_handle = rectangle('Position',[y,x,10,10],'FaceColor',[0.5 0 0],'EdgeColor','k',...
      'LineWidth',3);
  delete(msg_handle);

  % Get the goal string
  prompt = {'Enter a name for the env file'};
  dlg_title = 'Env file';
  num_lines = 1;
  def = {'1'};
  dlg_ans = inputdlg(prompt,dlg_title,num_lines,def);
  env_name = dlg_ans{1};

  % Convert to binary.
  env = uint8(double(env) / 255.0);

  % Write the env configuration to a .cfg file
  env_file = sprintf('%s.cfg',env_name);
  fid = fopen(env_file,'w+');
  fprintf(fid,'discretization(cells): %d %d\n',env_x,env_y);
  fprintf(fid,'obsthresh: %d\n',obsthresh);
  fprintf(fid,'cost_inscribed_thresh: %d\n',cost_inscribed_thresh);
  fprintf(fid,'cost_possibly_circumscribed_thresh: %d\n',cost_possibly_circumscribed_thresh);
  fprintf(fid,'cellsize(meters): %f\n',res);
  fprintf(fid,'nominalvel(mpersecs): %f\n',nominal_vel);
  fprintf(fid,'timetoturn45degsinplace(secs): %f\n',time_to_turn_45_degs);
  fprintf(fid,'start(meters,rads): %f %f %f\n',start_pt(1),start_pt(2),0);
  fprintf(fid,'end(meters,rads): %f %f %f\n',goal_pt(1),goal_pt(2),0);
  fprintf(fid,'environment:\n');
  fclose(fid);
  dlmwrite(env_file,env,'delimiter',' ','-append');

  close(fig_handle);
