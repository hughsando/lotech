dofile("../src/ltlua/lt.lua")

local scene = lt.Scene()
local tweens = lt.TweenSet()

lt.SetViewPort(-10, -10, 10, 10)

lt.dump_atlases = true

lt.LoadImages({
    "air_vent.png",
    "apple.png",
    "b.png",
    "banana.png",
    "bin_digit_0.png",
    "bin_digit_1.png",
    "black.png",
    "building.png",
    "building_night.png",
    "cabinet.png",
    "cabinet_drawers.png",
    "cabinet_large.png",
    "cabinet_prop.png",
    "cabinet_small.png",
    "cd.png",
    "ceiling_panel.png",
    "ceiling_tile2.png",
    "clock_face.png",
    "clock_face_prop.png",
    "clock_hour_hand.png",
    "clock_minute_hand.png",
    "clock_second_hand.png",
    "cloud.png",
    "coffee_pot.png",
    "coffee_pot_fill.png",
    "coffee_pot_highlight.png",
    "coffee_table.png",
    "coffee_table_prop.png",
    "congrats.png",
    "copier.png",
    "copier_prop.png",
    "debris1.png",
    "desk.png",
    "door.png",
    "dusk.png",
    "endless_hud_bg.png",
    "enemy2_hit_high_0001.png",
    "enemy2_hit_high_0002.png",
    "enemy2_hit_high_0003.png",
    "enemy2_hit_high_0004.png",
    "enemy2_hit_high_0005.png",
    "enemy2_hit_high_0006.png",
    "enemy2_hit_high_0007.png",
    "enemy2_hit_high_0008.png",
    "enemy2_hit_high_0009.png",
    "enemy2_hit_high_0010.png",
    "enemy2_hit_high_0011.png",
    "enemy2_hit_high_0012.png",
    "enemy2_hit_high_0013.png",
    "enemy2_hit_high_0014.png",
    "enemy2_hit_high_0015.png",
    "enemy2_hit_high_0016.png",
    "enemy2_hit_high_0017.png",
    "enemy2_hit_high_0018.png",
    "enemy2_standing.png",
    "enemy2_start_walk_0001.png",
    "enemy2_start_walk_0002.png",
    "enemy2_start_walk_0003.png",
    "enemy2_start_walk_0004.png",
    "enemy2_start_walk_0005.png",
    "enemy2_walk_0001.png",
    "enemy2_walk_0002.png",
    "enemy2_walk_0003.png",
    "enemy2_walk_0004.png",
    "enemy2_walk_0005.png",
    "enemy2_walk_0006.png",
    "enemy2_walk_0007.png",
    "enemy2_walk_0008.png",
    "enemy2_walk_0009.png",
    "enemy2_walk_0010.png",
    "enemy2_walk_0011.png",
    "enemy2_walk_0012.png",
    "enemy2_walk_0013.png",
    "enemy2_walk_0014.png",
    "enemy2_walk_0015.png",
    "enemy2_walk_0016.png",
    "enemy2_walk_0017.png",
    "enemy2_walk_0018.png",
    "enemy2_walk_0019.png",
    "enemy2_walk_0020.png",
    "enemy2_walk_0021.png",
    "enemy2_walk_0022.png",
    "enemy2_walk_0023.png",
    "enemy2_walk_0024.png",
    "enemy2_walk_0025.png",
    "enemy2_walk_0026.png",
    "enemy2_walk_0027.png",
    "enemy3_hit_high_0001.png",
    "enemy3_hit_high_0002.png",
    "enemy3_hit_high_0003.png",
    "enemy3_hit_high_0004.png",
    "enemy3_hit_high_0005.png",
    "enemy3_hit_high_0006.png",
    "enemy3_hit_high_0007.png",
    "enemy3_hit_high_0008.png",
    "enemy3_hit_high_0009.png",
    "enemy3_hit_high_0010.png",
    "enemy3_hit_high_0011.png",
    "enemy3_hit_high_0012.png",
    "enemy3_hit_high_0013.png",
    "enemy3_hit_high_0014.png",
    "enemy3_hit_high_0015.png",
    "enemy3_hit_high_0016.png",
    "enemy3_hit_high_0017.png",
    "enemy3_standing.png",
    "enemy3_start_walk_0001.png",
    "enemy3_start_walk_0002.png",
    "enemy3_start_walk_0003.png",
    "enemy3_start_walk_0004.png",
    "enemy3_start_walk_0005.png",
    "enemy3_walk_0001.png",
    "enemy3_walk_0002.png",
    "enemy3_walk_0003.png",
    "enemy3_walk_0004.png",
    "enemy3_walk_0005.png",
    "enemy3_walk_0006.png",
    "enemy3_walk_0007.png",
    "enemy3_walk_0008.png",
    "enemy3_walk_0009.png",
    "enemy3_walk_0010.png",
    "enemy3_walk_0011.png",
    "enemy3_walk_0012.png",
    "enemy3_walk_0013.png",
    "enemy3_walk_0014.png",
    "enemy3_walk_0015.png",
    "enemy3_walk_0016.png",
    "enemy3_walk_0017.png",
    "enemy3_walk_0018.png",
    "enemy3_walk_0019.png",
    "enemy3_walk_0020.png",
    "enemy3_walk_0021.png",
    "enemy3_walk_0022.png",
    "enemy3_walk_0023.png",
    "enemy3_walk_0024.png",
    "enemy3_walk_0025.png",
    "enemy3_walk_0026.png",
    "enemy3_walk_0027.png",
    "enemy4_hit_high_0001.png",
    "enemy4_hit_high_0002.png",
    "enemy4_hit_high_0003.png",
    "enemy4_hit_high_0004.png",
    "enemy4_hit_high_0005.png",
    "enemy4_hit_high_0006.png",
    "enemy4_hit_high_0007.png",
    "enemy4_hit_high_0008.png",
    "enemy4_hit_high_0009.png",
    "enemy4_hit_high_0010.png",
    "enemy4_hit_high_0011.png",
    "enemy4_hit_high_0012.png",
    "enemy4_hit_high_0013.png",
    "enemy4_hit_high_0014.png",
    "enemy4_hit_high_0015.png",
    "enemy4_hit_high_0016.png",
    "enemy4_special_0001.png",
    "enemy4_special_0002.png",
    "enemy4_special_0003.png",
    "enemy4_special_0004.png",
    "enemy4_special_0005.png",
    "enemy4_special_0006.png",
    "enemy4_special_0007.png",
    "enemy4_special_0008.png",
    "enemy4_special_0009.png",
    "enemy4_special_0010.png",
    "enemy4_special_0011.png",
    "enemy4_special_0012.png",
    "enemy4_special_0013.png",
    "enemy4_special_0014.png",
    "enemy4_special_0015.png",
    "enemy4_special_0016.png",
    "enemy4_special_0017.png",
    "enemy4_special_0018.png",
    "enemy4_special_0019.png",
    "enemy4_special_0020.png",
    "enemy4_special_0021.png",
    "enemy4_special_0022.png",
    "enemy4_special_0023.png",
    "enemy4_special_0024.png",
    "enemy4_special_0025.png",
    "enemy4_special_0026.png",
    "enemy4_special_0027.png",
    "enemy4_special_0028.png",
    "enemy4_special_glow.png",
    "enemy4_standing.png",
    "enemy4_start_walk_0001.png",
    "enemy4_start_walk_0002.png",
    "enemy4_start_walk_0003.png",
    "enemy4_start_walk_0004.png",
    "enemy4_start_walk_0005.png",
    "enemy4_start_walk_0006.png",
    "enemy4_walk_0001.png",
    "enemy4_walk_0002.png",
    "enemy4_walk_0003.png",
    "enemy4_walk_0004.png",
    "enemy4_walk_0005.png",
    "enemy4_walk_0006.png",
    "enemy4_walk_0007.png",
    "enemy4_walk_0008.png",
    "enemy4_walk_0009.png",
    "enemy4_walk_0010.png",
    "enemy4_walk_0011.png",
    "enemy4_walk_0012.png",
    "enemy4_walk_0013.png",
    "enemy4_walk_0014.png",
    "enemy4_walk_0015.png",
    "enemy4_walk_0016.png",
    "enemy4_walk_0017.png",
    "enemy4_walk_0018.png",
    "enemy4_walk_0019.png",
    "enemy4_walk_0020.png",
    "enemy4_walk_0021.png",
    "enemy4_walk_0022.png",
    "enemy4_walk_0023.png",
    "enemy4_walk_0024.png",
    "enemy4_walk_0025.png",
    "enemy4_walk_0026.png",
    "enemy4_walk_0027.png",
    "enemy_hit_high_0001.png",
    "enemy_hit_high_0002.png",
    "enemy_hit_high_0003.png",
    "enemy_hit_high_0004.png",
    "enemy_hit_high_0005.png",
    "enemy_hit_high_0006.png",
    "enemy_hit_high_0007.png",
    "enemy_hit_high_0008.png",
    "enemy_hit_high_0009.png",
    "enemy_hit_high_0010.png",
    "enemy_hit_high_0011.png",
    "enemy_hit_high_0012.png",
    "enemy_hit_high_0013.png",
    "enemy_hit_high_0014.png",
    "enemy_hit_high_0015.png",
    "enemy_hit_high_0016.png",
    "enemy_hit_high_0017.png",
    "enemy_hit_high_0018.png",
    "enemy_standing.png",
    "enemy_start_walk_0001.png",
    "enemy_start_walk_0002.png",
    "enemy_start_walk_0003.png",
    "enemy_start_walk_0004.png",
    "enemy_start_walk_0005.png",
    "enemy_walk_0001.png",
    "enemy_walk_0002.png",
    "enemy_walk_0003.png",
    "enemy_walk_0004.png",
    "enemy_walk_0005.png",
    "enemy_walk_0006.png",
    "enemy_walk_0007.png",
    "enemy_walk_0008.png",
    "enemy_walk_0009.png",
    "enemy_walk_0010.png",
    "enemy_walk_0011.png",
    "enemy_walk_0012.png",
    "enemy_walk_0013.png",
    "enemy_walk_0014.png",
    "enemy_walk_0015.png",
    "enemy_walk_0016.png",
    "enemy_walk_0017.png",
    "enemy_walk_0018.png",
    "enemy_walk_0019.png",
    "enemy_walk_0020.png",
    "enemy_walk_0021.png",
    "enemy_walk_0022.png",
    "enemy_walk_0023.png",
    "enemy_walk_0024.png",
    "enemy_walk_0025.png",
    "enemy_walk_0026.png",
    "enemy_walk_0027.png",
    "energy_bar_fill.png",
    "energy_bar_fill_alt.png",
    "extinguisher.png",
    "eye_glow.png",
    "fax.png",
    "floor.png",
    "floor_gradient.png",
    "floor_gradient2.png",
    "floor_reached.png",
    "g.png",
    "game_over.png",
    "gameover_menu_button.png",
    "gameover_menu_button_on.png",
    "gameover_retry_button.png",
    "gameover_retry_button_on.png",
    "gradient1.png",
    "green_pixel.png",
    "health_bar1.png",
    "health_bar2.png",
    "health_bar_fill.png",
    "health_bar_fill_alt.png",
    "job_complete.png",
    "kettle.png",
    "keyboard.png",
    "laser.png",
    "laser1.png",
    "laser2.png",
    "laser3.png",
    "laser4.png",
    "led_d_off.png",
    "led_d_on.png",
    "led_h_off.png",
    "led_h_on.png",
    "led_v_off.png",
    "led_v_on.png",
    "lol_debris.png",
    "lol_flash.png",
    "loo_f.png",
    "loo_m.png",
    "monitor.png",
    "mouse.png",
    "mug.png",
    "neon_tile.png",
    "office_chair.png",
    "omg_debris.png",
    "omg_flash.png",
    "painting.png",
    "painting_prop.png",
    "paper_plane.png",
    "pause.png",
    "paused.png",
    "pc_tower.png",
    "pixel_debris.png",
    "plant.png",
    "player_die_0001.png",
    "player_die_0002.png",
    "player_die_0003.png",
    "player_die_0004.png",
    "player_die_0005.png",
    "player_die_0006.png",
    "player_die_0007.png",
    "player_die_0008.png",
    "player_die_0009.png",
    "player_die_0010.png",
    "player_die_0011.png",
    "player_duck_end_0001.png",
    "player_duck_end_0002.png",
    "player_duck_end_0003.png",
    "player_duck_end_0004.png",
    "player_duck_end_0005.png",
    "player_duck_start_0001.png",
    "player_duck_start_0002.png",
    "player_duck_start_0003.png",
    "player_duck_start_0004.png",
    "player_duck_start_0005.png",
    "player_duck_start_0006.png",
    "player_hit_0001.png",
    "player_hit_0002.png",
    "player_hit_0003.png",
    "player_hit_0004.png",
    "player_hit_0005.png",
    "player_hit_0006.png",
    "player_hit_0007.png",
    "player_hit_0008.png",
    "player_hit_0009.png",
    "player_hit_0010.png",
    "player_hit_0011.png",
    "player_hit_0012.png",
    "player_punch_0001.png",
    "player_punch_0002.png",
    "player_punch_0003.png",
    "player_punch_0004.png",
    "player_punch_0005.png",
    "player_punch_0006.png",
    "player_punch_0007.png",
    "player_punch_0008.png",
    "player_punch_0009.png",
    "player_punch_0010.png",
    "player_punch_0011.png",
    "player_punch_0012.png",
    "player_punch_0013.png",
    "player_punch_0014.png",
    "player_punch_0015.png",
    "player_punch_0016.png",
    "player_punch_0017.png",
    "player_punch_0018.png",
    "player_punch_0019.png",
    "player_punch_0020.png",
    "player_punch_0021.png",
    "player_punch_0022.png",
    "player_punch_0023.png",
    "player_punch_0024.png",
    "player_standing.png",
    "player_standing_hand.png",
    "player_throw.png",
    "player_throw_heavy_0007.png",
    "player_throw_heavy_0008.png",
    "player_throw_heavy_0009.png",
    "player_throw_heavy_0010.png",
    "player_throw_heavy_0011.png",
    "player_throw_heavy_0012.png",
    "player_throw_heavy_0013.png",
    "player_throw_heavy_0014.png",
    "player_throw_heavy_0015.png",
    "player_throw_heavy_0016.png",
    "player_tired_0001.png",
    "player_tired_0002.png",
    "player_tired_0003.png",
    "player_tired_0004.png",
    "player_tired_0005.png",
    "player_win_0001.png",
    "player_win_0002.png",
    "player_win_0003.png",
    "player_win_0004.png",
    "player_win_0005.png",
    "player_win_0006.png",
    "player_win_0007.png",
    "player_win_0008.png",
    "player_win_0009.png",
    "player_win_0010.png",
    "player_win_0011.png",
    "player_win_0012.png",
    "player_win_0013.png",
    "player_win_0014.png",
    "player_win_0015.png",
    "player_win_0016.png",
    "player_win_0017.png",
    "player_win_0018.png",
    "player_win_0019.png",
    "player_win_0020.png",
    "player_win_0021.png",
    "player_win_0022.png",
    "player_win_0023.png",
    "player_win_0024.png",
    "player_win_0025.png",
    "player_win_0026.png",
    "player_win_0027.png",
    "player_win_return_0001.png",
    "player_win_return_0002.png",
    "player_win_return_0003.png",
    "player_win_return_0004.png",
    "player_win_return_0005.png",
    "player_win_return_0006.png",
    "player_win_return_0007.png",
    "player_win_return_0008.png",
    "player_win_return_0009.png",
    "player_win_return_0010.png",
    "player_win_return_0011.png",
    "player_win_return_0012.png",
    "player_zap.png",
    "pow_0001.png",
    "pow_0002.png",
    "pow_0003.png",
    "pow_0004.png",
    "pow_0005.png",
    "pow_bg.png",
    "printer_prop.png",
    "pu_glow.png",
    "pu_health.png",
    "pu_nuke.png",
    "pu_quaddamage.png",
    "pu_rapidfire.png",
    "pu_tornado.png",
    "pu_win_activated.png",
    "pu_win_bg_expired.png",
    "pu_win_msg_health.png",
    "pu_win_msg_nuke.png",
    "pu_win_msg_quad.png",
    "pu_win_msg_rapidfire.png",
    "pu_win_msg_tornado.png",
    "pu_win_progress_bar.png",
    "quit.png",
    "quit_on.png",
    "red_pixel.png",
    "resume.png",
    "resume_on.png",
    "score0.png",
    "score1.png",
    "score2.png",
    "score3.png",
    "score4.png",
    "score5.png",
    "score6.png",
    "score7.png",
    "score8.png",
    "score9.png",
    "score_lockup.png",
    "shadow.png",
    "shutters.png",
    "sky.png",
    "smartphone.png",
    "smoke.png",
    "sofa.png",
    "sofa_prop.png",
    "spider_boss_1.png",
    "spider_boss_2.png",
    "spider_boss_hit.png",
    "spider_small.png",
    "staff_neutralized.png",
    "stapler.png",
    "story_hud_bg.png",
    "swiss_ball.png",
    "swiss_ball_highlight.png",
    "table.png",
    "wall_gradient.png",
    "wall_gradient2.png",
    "wall_gradient3.png",
    "water_cooler.png",
    "water_cooler_prop.png",
    "whisky.png",
    "whiteboard_prop.png",
    "win_menu_button_down.png",
    "win_menu_button_up.png",
    "win_popup.png",
    "win_retry_button_down.png",
    "win_retry_button_up.png",
    "win_text.png",
    "window_frame_gradient.png",
    "workstation.png",
    "z.png"
})

--[[
lt.LoadImages({
    "arrow.png",
    "background1.png",
    "background2.png",
    "background3.png",
    "background4.png",
    "background5.png",
    "ball.png",
    "ball_bw.png",
    "bee1.png",
    "bee2.png",
    "bee3.png",
    "butterfly1.png",
    "butterfly2.png",
    "butterfly_rare1.png",
    "butterfly_rare2.png",
    "credits.png",
    "daisies2.png",
    "daisies3.png",
    "daisies4a.png",
    "daisies4b.png",
    "daisies5a.png",
    "daisies5b.png",
    "digit0.png",
    "digit1.png",
    "digit2.png",
    "digit3.png",
    "digit4.png",
    "digit5.png",
    "digit6.png",
    "digit7.png",
    "digit8.png",
    "digit9.png",
    "menu.png",
    "menu_difficulty.png",
    "menu_difficulty_easy.png",
    "menu_difficulty_hard.png",
    "menu_difficulty_medium.png",
    "menu_music.png",
    "menu_music_off.png",
    "menu_music_on.png",
    "menu_openfeint.png",
    "menu_openfeint_off.png",
    "menu_openfeint_on.png",
    "menu_resume.png",
    "pause.png",
    "plane.png",
    "plane_overlay.png",
    "shadow.png",
    "text_best.png",
    "text_instructions.png",
    "text_score.png",
    "text_title.png"
})
]]

local imgs = {}
local n = 0
for _, img in pairs(lt.images) do
    n = n + 1
    imgs[n] = img
end

local i = 1

function lt.KeyDown(key)
    if key == "right" then
        i = i + 1
    elseif key == "left" then
        i = i - 1
    end
    if i > n then
        i = 1
    elseif i < 1 then
        i = n
    end
end

function lt.Advance()
end

function lt.Render()
    lt.Scale(20)
    lt.SetColor(0.5, 0.5, 0.5)
    lt.DrawRect(-10, -10, 10, 10)
    lt.SetColor(1, 1, 1)
    imgs[i]:DrawBottomLeft()
end
