-- -*- sql -*-
 
--    $Id$

--  Copyright 1996 X=X Computer Software Trust
--   		  Kangaroo Ground Australia 3097


-- This is free software; you can redistribute it and/or modify
-- it under the terms of the GNU General Public Licensepublished by
-- the Free Software Foundation; either version 2, or (at your option)
-- any later version.
 
-- This software is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
 
-- You should have received a copy of the GNU General Public License
-- along with this software; see the file COPYING.  If not, write to
-- the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 
-- Report problems and direct all questions to:
 
--     Rex McMaster, rmcm@compsoft.com.au
 

 
-- Revision Log
  
--
-- PL function to set defaults if null
--
create or replace function bkdp_tr_before()
returns trigger
as 'DECLARE
       tmp_bkdp_amount bkdp.bkdp_amount%TYPE; 
    BEGIN
      if (TG_OP = ''DELETE'') then
        RAISE EXCEPTION ''You cannot remove banking batches'';
        return null;
      end if;


      select    sum(paym_amount)
      into      tmp_bkdp_amount
      from      paym
      where     paym_bkdp__sequence = new.bkdp__sequence;

    if ( tmp_bkdp_amount is null ) then
      new.bkdp_amount := 0.00::numeric;
    else
      new.bkdp_amount := tmp_bkdp_amount;
    end if;

    return new;

    END;'
    LANGUAGE 'plpgsql';

drop trigger bkdp_tr_before on bkdp;
create trigger bkdp_tr_before before insert or update or delete
    on bkdp for each row
    execute procedure bkdp_tr_before();


-- update history;
\set mttb_name '\'bkdp\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
