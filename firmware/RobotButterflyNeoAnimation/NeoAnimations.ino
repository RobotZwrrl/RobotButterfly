
void initNeoAnimations() {
  initNeoAnim_none(&neo_animations[NEO_ANIM_NONE]);
  initNeoAnim_polkadot(&neo_animations[NEO_ANIM_POLKADOT]);
  initNeoAnim_squiggle(&neo_animations[NEO_ANIM_SQUIGGLE]);
}


void initNeoAnim_none(struct NeoAnimation *a) {
  a->id = NEO_ANIM_NONE;
  a->active = false;
  a->type = NEO_ANIM_ALERT;

  a->num_frames = 1;
  a->frame_delay = 100;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = 1;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = 80;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 0;
  a->helper2 = 0;
  a->helper3 = 0;

  a->function = runNeoAnim_none;
}


void initNeoAnim_polkadot(struct NeoAnimation *a) {
  a->id = NEO_ANIM_POLKADOT;
  a->active = false;
  a->type = NEO_ANIM_ALERT;

  a->num_frames = 2;
  a->frame_delay = 100;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 0;
  a->helper2 = 0;
  a->helper3 = 0;

  a->function = runNeoAnim_polkadot;
}


void initNeoAnim_squiggle(struct NeoAnimation *a) {
  a->id = NEO_ANIM_SQUIGGLE;
  a->active = false;
  a->type = NEO_ANIM_ALERT;

  a->num_frames = 1;
  a->frame_delay = 100;
  a->frame_index = 0;
  a->last_frame = 0;

  a->num_repeats = -99;
  a->repeat_count = 0;
  a->repeat_delay = 0;
  a->last_repeat = 0;

  a->duration = -1;
  a->start_time = -1;

  a->dir = true;
  a->helper1 = 0;
  a->helper2 = 0;
  a->helper3 = 0;

  a->function = runNeoAnim_squiggle;
}


