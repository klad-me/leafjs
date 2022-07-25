Array.prototype.fill = function(value)
{
	for (var i=0; i<this.length; i++)
		this[i]=value;
	return this;
}
