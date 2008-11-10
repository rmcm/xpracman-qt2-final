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
  
--
-- PL function to set defaults if null
--
drop trigger tr_mdbt_update on mdaf;
drop function fn_mdbt_update();
create function fn_mdbt_update()
returns opaque
as '
        DECLARE

                x_mdbt__sequence mdbt.mdbt__sequence%TYPE;

        BEGIN
        -- 
        if (TG_OP = ''DELETE'') then
          x_mdbt__sequence := old.mdaf_mdbt__sequence;
        else
          x_mdbt__sequence := new.mdaf_mdbt__sequence;
        end if;

        -- update statistics
        if (x_mdbt__sequence is not null) then
          update  mdbt
          set     mdbt_voucher_count = (select count(*) from mdaf where mdaf_mdbt__sequence = x_mdbt__sequence),
                  mdbt_total_amount = (select coalesce(sum(svpf_amount),0.0) + coalesce(sum(svpf_gst_amount), 0.0)
                                          from    svpf
                                          where   svpf_mdaf__sequence in (select  mdaf__sequence
                                                                          from    mdaf
                                                                          where   mdaf_mdbt__sequence = x_mdbt__sequence))
          where   mdbt__sequence = x_mdbt__sequence;
        end if;
  
        if ( TG_OP = ''DELETE'' ) then
          return old;
        else
          return new;
        end if;

        END;'
    LANGUAGE 'plpgsql';

create trigger tr_mdbt_update after insert or update or delete
    on mdaf for each row
    execute procedure fn_mdbt_update();

-- update history;
\set mttb_name '\'mdaf\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;


