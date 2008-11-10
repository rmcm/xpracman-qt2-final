--     $Id$
-- 
--   Copyright 2006 X=X Computer Software Trust
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



-- ----------------------------------------
-- Audit changes to patn table
-- ----------------------------------------

create or replace function fn_patn_audit(integer, text, patn, patn)
  returns boolean as $$

  DECLARE
    
  BEGIN

    RAISE NOTICE '% :: %', $3.patn_psnam, $4.patn_psnam;
    if ($3.patn_psnam != $4.patn_psnam ) then
      RAISE NOTICE 'patn_psnam is different';
      PERFORM fn_mtau_insert('patn'::text, $1, $2, 'patn_psnam', $3.patn_psnam, $4.patn_psnam);
    end if;                                  
                                  
  RETURN true;

  END;

$$ LANGUAGE 'plpgsql';

-- ----------------------------------------
-- Audit changes to patn table - trigger
-- ----------------------------------------

create or replace function fn_patn_audit()
returns trigger as $$

  BEGIN

    RAISE NOTICE 'trigger fn_patn_audit :: %', old.patn__sequence;
    PERFORM fn_patn_audit(old.patn__sequence, TG_OP, old, new);
    return new;

  END;

$$ LANGUAGE 'plpgsql';

drop trigger tr_patn_audit on patn;
create trigger tr_patn_audit after update
    on patn for each row
    execute procedure fn_patn_audit();

