function require(name)
{
	var fullName=path.resolve(name);
	if (! fullName) throw new Error(name+': module not found');
	return require.cache[fullName] || (function(exports) { eval(fs.readFileSync(name).toString()); return exports; })(require.cache[fullName] = {});
}
require.cache = Object.create(null);
