
void initNeoAnimations() {
  initNeoAnim_none();
  initNeoAnim_polkadot();
  initNeoAnim_squiggle();
}


void initNeoAnim_none() {
  neoanim_none.id = NEO_ANIM_NONE;
  neoanim_none.colour_primary = colourPalette[NEO_OFF];
  neoanim_none.colour_secondary = colourPalette[NEO_OFF];
  neoanim_none.active = false;

  neoanim_none.num_frames = 1;
  neoanim_none.frame_index = 0;
  neoanim_none.last_frame = 0;
  neoanim_none.frame_delay = 100;

  neoanim_none.num_repeats = 1;
  neoanim_none.duration = 80;
  neoanim_none.repeat_count = 0;
  neoanim_none.start_time = -1;
  neoanim_none.last_repeat = 0;
  neoanim_none.repeat_delay = 0;

  neoanim_none.dir = true;
  neoanim_none.helper1 = 0;
  neoanim_none.helper2 = 0;
  neoanim_none.helper3 = 0;

  neoanim_none.function = runNeoAnim_none;

  all_neo_animations[NEO_ANIM_NONE] = neoanim_none;
}


void initNeoAnim_polkadot() {
  neoanim_polkadot.id = NEO_ANIM_POLKADOT;
  neoanim_polkadot.colour_primary = colourPalette[NEO_MAGENTA];
  neoanim_polkadot.colour_secondary = colourPalette[NEO_CYAN];
  neoanim_polkadot.active = false;

  neoanim_polkadot.num_frames = 2;
  neoanim_polkadot.frame_index = 0;
  neoanim_polkadot.last_frame = 0;
  neoanim_polkadot.frame_delay = 100;

  neoanim_polkadot.num_repeats = -99;
  neoanim_polkadot.repeat_count = 0;
  neoanim_polkadot.last_repeat = 0;
  neoanim_polkadot.repeat_delay = 0;

  neoanim_polkadot.dir = true;
  neoanim_polkadot.helper1 = 0;
  neoanim_polkadot.helper2 = 0;
  neoanim_polkadot.helper3 = 0;

  neoanim_polkadot.function = runNeoAnim_polkadot;

  all_neo_animations[NEO_ANIM_POLKADOT] = neoanim_polkadot;
}


void initNeoAnim_squiggle() {
  neoanim_squiggle.id = NEO_ANIM_SQUIGGLE;
  neoanim_squiggle.colour_primary = colourPalette[NEO_MAGENTA];
  neoanim_squiggle.colour_secondary = colourPalette[NEO_CYAN];
  neoanim_squiggle.active = false;

  neoanim_squiggle.num_frames = 1;
  neoanim_squiggle.frame_index = 0;
  neoanim_squiggle.last_frame = 0;
  neoanim_squiggle.frame_delay = 100;

  neoanim_squiggle.num_repeats = -99;
  neoanim_squiggle.repeat_count = 0;
  neoanim_squiggle.last_repeat = 0;
  neoanim_squiggle.repeat_delay = 0;

  neoanim_squiggle.dir = true;
  neoanim_squiggle.helper1 = 0;
  neoanim_squiggle.helper2 = 0;
  neoanim_squiggle.helper3 = 0;

  neoanim_squiggle.function = runNeoAnim_squiggle;

  all_neo_animations[NEO_ANIM_SQUIGGLE] = neoanim_squiggle;
}


