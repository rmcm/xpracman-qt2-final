--     $Id$
--
--   Copyright 2009 X=X Computer Software Trust
--                Kangaroo Ground Australia 3097
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

-- ------------------------------------------------------------
-- check appointment dates - limit to range
-- ------------------------------------------------------------

create or replace function fn_limitdaterange()
returns trigger
as $$
    DECLARE
    -- NAME TABLE.ATTRIBUTE%TYPE;
    -- NAME TABLE%ROWTYPE;
    x_period interval;
    x_period_str text;

    rec_evnt evnt%ROWTYPE;

    BEGIN
        -- RAISE NOTICE 'fn_limitdaterange: evnt__sequence = %', new.evnt__sequence;

        -- ----------------------------------------
        -- if timestamp out of range then raise
        -- raise an exception
        -- ----------------------------------------
        if ( new.evnt_starttime is not null and 
              ( (new.evnt_starttime != old.evnt_starttime )
               or (old.evnt_starttime is null ) ) ) then
          select conf_value::interval, conf_value
          into   x_period, x_period_str
          from   conf
          where conf_code = 'EVNT_STARTTIME_RANGE';
          if ( found ) then
            if ( new.evnt_starttime < 'today'::timestamp 
               or new.evnt_starttime > now() + x_period ) then
              RAISE EXCEPTION '% is outside the date range limits:<br> Now until % into the future', new.evnt_starttime, x_period_str;
              return null;
            end if;
          end if;
        end if;

    return new;

    END;
  $$ LANGUAGE 'plpgsql';

drop trigger tr_limitdaterange on evnt;
create trigger tr_limitdaterange before update
    on evnt for each row
    execute procedure fn_limitdaterange();

-- add conf entry

insert  into conf(conf_code,conf_desc,conf_value)
select  'EVNT_STARTTIME_RANGE', 'Interval into future for valid appointment', '12 months'
where   'EVNT_STARTTIME_RANGE' not in (select conf_code from conf);

-- update history;
\set mttb_name '\'evnt\''
\set mttb_rcs_header '\'$Id$\''
insert into mttb(mttb_name, mttb_rcs_header) select :mttb_name, :mttb_rcs_header
        except select mttb_name, mttb_rcs_header from mttb;
