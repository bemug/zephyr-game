#ifndef AIRCRAFT_H
#define AIRCRAFT_H

class Aircraft : public Entity
{
	public:
		enum Type
		{
			Eagle,
			Raptor,
		};

	public:
		explicit Aircraft(Type type); //Explicit means the "type" paramter has to be a proper type (not implicitly converted)

	private:
		Type mType;
};

#endif
