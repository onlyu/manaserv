----------------------------------------------------------
-- Postman Function Sample                                      --
----------------------------------------------------------------------------------
--  Copyright 2009-2010 The Mana World Development Team                              --
--                                                                              --
--  This file is part of The Mana World.                                        --
--                                                                              --
--  The Mana World  is free software; you can redistribute  it and/or modify it --
--  under the terms of the GNU General  Public License as published by the Free --
--  Software Foundation; either version 2 of the License, or any later version. --
----------------------------------------------------------------------------------

function post_talk(npc, ch)
  mana.npc_message(npc, ch, "Hello " .. mana.being_get_name(ch))
  local strength = mana.being_get_attribute(ch, ATTR_STRENGTH)
  mana.npc_message(npc, ch, "You have " .. tostring(strength) .. " strength")
  mana.npc_message(npc, ch, "What would you like to do?")
  local answer = mana.npc_choice(npc, ch, "View Mail", "Send Mail", "Nothing")
  if answer == 1 then
    local sender, post = mana.chr_get_post(ch)
    if sender == "" then
      mana.npc_message(npc, ch, "No Post right now, sorry")
    else
      mana.npc_message(npc, ch, tostring(sender) .. " sent you " .. tostring(post))
    end
  end
  if answer == 2 then
    mana.npc_post(npc, ch)
  end
end
