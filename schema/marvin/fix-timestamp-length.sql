-- set timestamps (non-system) to length 10

update mtat set mtat_displen = 10 where mtat_type = 'timestamp' and mtat_name !~ '__timestamp';
