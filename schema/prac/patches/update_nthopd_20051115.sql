--     $Id$
-- 
--   Copyright 2005 X=X Computer Software Trust
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


-- Update XPracMan Database 2005-11-15

\i schema/marvin/trigger_utils.sql 
\i schema/prac/empl-triggers.sql
\i schema/prac/svpf_annotation.sql
\i schema/prac/empl_notes_phone.sql
\i schema/prac/patn_last_note.sql
\i schema/prac/patn_more_phones.sql
\i schema/prac/svpf-functions.sql
\i schema/prac/svpf-triggers.sql
\i schema/prac/patn-triggers.sql
\i schema/prac/dbtr-functions.sql
update mtat set mtat_input_method = 'SYSTEM' where mtat_name ~ '^patn__.*';
