-- Extended code from : Mike Finn <mike.finn@tacticalExecutive.com>
-- Get the Nth parm from a parameter set stored as null delimited
-- bytea as a string.  Parameters are numbered from 0
drop function byteaparam(bytea, int);
create function byteaparam(bytea, int) returns text as '
    declare
        bparm   alias for $1;  --bytea parameters
        parmIdx alias for $2;

        parmCount  int = 0;
        charCount  int = 0;
        bsize      int;
        c          int;
        parm       text = '''';

    begin
        -- length of bytea
        select octet_length(bparm)
        into   bsize;

        -- skip over the leading params
        while parmCount < parmIdx loop
            if (charCount >= bsize) then
               return '''';
            end if;
            c := get_byte(bparm, charCount);
            charCount := charCount + 1;
            if c = 0 then
                parmCount := parmCount + 1;
            end if;
        end loop;

        -- extract the parameter.
        while parmCount = parmIdx loop
            if (charCount >= bsize) then
               return '''';
            end if;
            c := get_byte(bparm, charCount);
            charCount := charCount + 1;
            if c = 0 then
                parmCount := parmCount + 1;
            else
                parm := parm || chr(c);
            end if;
        end loop;

        return parm;
    end;
' language 'plpgsql';

