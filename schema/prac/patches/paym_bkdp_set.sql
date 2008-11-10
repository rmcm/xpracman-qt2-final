--     $Id$
-- 
--   Copyright 2004 X=X Computer Software Trust
--    		  Kangaroo Ground Australia 3097
-- 
-- 
--  This is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License published by
--  the Free Software Foundation; either version 2, or (at your option)
--  any later version.
--  
--  This software is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--  
--  You should have received a copy of the GNU General Public License
--  along with this software; see the file COPYING.  If not, write to
--  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
--  
--  Report problems and direct all questions to:
--  
--      Rex McMaster, rmcm@compsoft.com.au
--  
-- 


-- create a set of monthly bank-batches from a paym table;

-- disable triggers
select disable_triggers();

-- create the monthly bank batches
insert into bkdp(bkdp_date_created, bkdp_desc)
  select distinct date(date_trunc('month', paym_date_entry)), 
  'Auto-batched '||to_char(paym_date_entry,'YYYY, Month') from paym
  except select bkdp_date_created, bkdp_desc from bkdp;

-- update paym to associate with the correct batch

update paym
  set paym_bkdp__sequence = bkdp.bkdp__sequence
  from bkdp
  where date(bkdp_date_created) = date(date_trunc('month', paym_date_entry));

-- restore triggers
select enable_triggers();

-- update balances
update bkdp set bkdp_desc = bkdp_desc;
