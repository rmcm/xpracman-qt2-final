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



-- add an attribute - last service, updated via a trigger from the SVPF table.

alter table patn add column patn_last_service timestamp;

-- ------------------------------------------------------------
-- Update patient "last service"
-- ------------------------------------------------------------

create or replace function fn_lastservice()
returns trigger
as 'DECLARE
      x_patn__sequence patn.patn__sequence%TYPE;

    BEGIN
        if (TG_OP = ''INSERT'' ) then
          if (new.svpf_date_service is null) then
            return new;
          end if;
          x_patn__sequence = new.svpf_patn__sequence;
        end if;

        if (TG_OP = ''UPDATE'' ) then
          if (new.svpf_date_service = old.svpf_date_service) then
            return new;
          end if;
          x_patn__sequence = new.svpf_patn__sequence;
        end if;

        if (TG_OP = ''DELETE'' ) then
          x_patn__sequence = old.svpf_patn__sequence;
        end if;

        UPDATE  patn
        set     patn_last_service = (
                        select  max(svpf_date_service)
                        from    svpf
                        where   svpf_patn__sequence = x_patn__sequence )
        where   patn__sequence = x_patn__sequence;
        -- RAISE NOTICE ''setting patn_last_service for %'', x_patn__sequence;

    if (TG_OP = ''INSERT'' or TG_OP = ''UPDATE'' ) then
      return new;
    else
      return old;
    end if;

    END;'
  LANGUAGE 'plpgsql';

drop trigger tr_lastservice on svpf;
create trigger tr_lastservice after insert or update or delete
    on svpf for each row
    execute procedure fn_lastservice();

-- add patn_last_service
delete from mtat where mtat_name = 'patn_last_service';
insert into mtat(mtat_name, mtat_type, mtat_title, mtat_class_name, mtat_displen, mtat_browse_order, mtat_view_order, mtat_input_method) values('patn_last_service', 'timestamp', 'Last Service', 'patn', 30, 360, 360, 'RO');

-- update history;
\set mttb_name '\'patn\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;

