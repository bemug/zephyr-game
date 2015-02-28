#ifndef STATE_H
#define STATE_H

class State
{
	public:
		typedef std::unique_ptr<State> Ptr;
		struct Context {
			//TODO
		};
	public:
		State(StateStack& stack, Context context);
		virtual ~State();

		virtual bool draw() = 0;
		virtual bool update(sf::Time dt) = 0;
		virtual bool handleEvent(const sf::Event& event) = 0;

	protected:
		void requestStackPush(States::ID stateID);
		void requestStackPop();
		void requestStateClear();

		Context getContext() const;

	private:
		StateStack* mStack;
		Context mContext;
};

#endif
