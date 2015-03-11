class UsersController < ApplicationController
	def index
		@users = User.all()
	end
	def login
	end
	def post_login
		if User.where(:login => params[:username]).blank?
			flash[:noid] = "ID does not exist"
			redirect_to "/users/login"
		else
			@current_user = User.find_by_login(params[:username])
			if @current_user.password_valid? (params[:secret])
				session[:current_user_id] = @current_user.id
				x = @current_user.id
				redirect_to "/photos/index/" + x.to_s
			else
				flash[:wrongpassword] = "Wrong password"
				redirect_to "/users/login"
			end
		end
	end
	def logout
		session[:current_user_id] = nil
		redirect_to "/users/login"
	end
	def new
	end
	def create
		newuser = User.new
		newuser.first_name = params[:firstname]
		newuser.last_name = params[:lastname]
		newuser.login = params[:user_id]
		newuser.password = params[:password]
		if newuser.save() && (params[:password] == params[:password_confirmation])
			redirect_to "/users/login"
		else
			newuser.destroy()
			redirect_to "/users/new"
			if newuser.save()
				flash[:passwordunconfirmed] = "Passwords should match."
			else
				flash[:idexists] = "Username taken."
			end
		end
	end
end
